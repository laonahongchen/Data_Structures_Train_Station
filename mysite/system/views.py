from django.shortcuts import render
from system.forms import UserForm, UserProfileForm

from django.contrib.auth import authenticate, login, logout
from django.http import HttpResponseRedirect, HttpResponse
from django.urls import reverse

import os
import ctypes

def getServerSideCookie(request, cookie, default_val=None):
    val = request.session.get(cookie)
    if not val:
        val = default_val
    return val

def index(request):
    lib = ctypes.cdll.LoadLibrary('./lib/libcr.so')
    string_buffer = ctypes.create_string_buffer(1000)
    pointer = (ctypes.c_char_p)(ctypes.addressof(string_buffer))
    lib.searchTicket(pointer)
    order = ['车次', '出发站', '到达站', '出发时间', '到达时间', '软卧', '一等座', '二等座', '无座']
    post_list = []
    for string in string_buffer.value.decode('UTF-8').split('|'):
        info = string.split()
        name = info[0::2]
        value = info[1::2]
        ab = {'软卧':'--', '一等座':'--', '二等座':'--', '无座':'--'}
        for i in range(0, len(name)):
            ab[name[i]] = value[i]
        post = []
        id = 0
        for name in order:
            value = ab[name]
            id += 1
            textclass = ''
            if id <= 4:
                textclass = 'font-weight-bold'
            elif id == 5:
                textclass = 'text-secondary-bold'
            elif value == '--':
                textclass = 'text-muted'
            elif value == '0':
                value = '无'
                textclass = 'text-muted'
            elif int(value) <= 50:
                textclass = 'font-weight-bold'
            else:
                value = '有'
                textclass = 'text-success'
            print((value, textclass))
            post.append((value, textclass))
        post_list.append(post)
    print(post_list)
    wade = 'class=text-success'

    userid = getServerSideCookie(request, 'userid', '0')
    return render(request, 'chinarailway/index.html', context = {'post_list':post_list, 'class_type':'text-success', 'wade':wade, 'userid':userid})

def checkbox(s):
    if s == 'on':
        return 'checked'
    return ''

def booking(request):
    userid = getServerSideCookie(request, 'userid', '0')

    if request.method == 'POST':
        fromStation = request.POST.get('fromStation')
        toStation = request.POST.get('toStation')
        G = checkbox(request.POST.get('G'))
        D = checkbox(request.POST.get('D'))
        T = checkbox(request.POST.get('T'))
        K = checkbox(request.POST.get('K'))
        C = checkbox(request.POST.get('C'))
        return render(request, 'chinarailway/booking.html', context = {'fromStation':fromStation, 'toStation':toStation, 'G':G, 'D':D, 'T':T, 'K':K, 'C':C})
    else:
        G = D = T = K = C = 'checked'
        return render(request, 'chinarailway/booking.html', context = {'G':G, 'D':D, 'T':T, 'K':K, 'C':C})

def register(request):
    userid = getServerSideCookie(request, 'userid', '0')
    if userid != '0':
        return HttpResponseRedirect(reverse('cr_index'))

    registered = False
    if request.method == 'POST':
        username = request.POST.get('username')
        emailaddress = request.POST.get('emailaddress')
        phonenumber = request.POST.get('phonenumber')
        password = request.POST.get('password')

        lib = ctypes.cdll.LoadLibrary('./lib/crsystem/libcr.so')
        dataInput = ctypes.create_string_buffer(' '.join((username, password, emailaddress, phonenumber)).encode('UTF-8'), 1000)
        dataOutput = ctypes.create_string_buffer(1000)
        inputPointer = (ctypes.c_char_p)(ctypes.addressof(dataInput))
        outputPointer = (ctypes.c_char_p)(ctypes.addressof(dataOutput))
        lib.userRegister(inputPointer, outputPointer)
        info = dataOutput.value.decode('UTF-8')

        if info != '-1':
            return HttpResponseRedirect(reverse('cr_index'))

    return render(request, 'chinarailway/register.html', context = {'userid':userid})

def user_login(request):
    userid = getServerSideCookie(request, 'userid', '0')
    if userid != '0':
        return HttpResponseRedirect(reverse('cr_index'))

    if request.method == 'POST':
        userid = request.POST.get('userid')
        password = request.POST.get('password')

        lib = ctypes.cdll.LoadLibrary('./lib/crsystem/libcr.so')
        dataInput = ctypes.create_string_buffer(' '.join((userid, password)).encode('UTF-8'), 1000)
        dataOutput = ctypes.create_string_buffer(1000)
        inputPointer = (ctypes.c_char_p)(ctypes.addressof(dataInput))
        outputPointer = (ctypes.c_char_p)(ctypes.addressof(dataOutput))
        lib.userLogin(inputPointer, outputPointer)
        info = dataOutput.value.decode('UTF-8')

        if info == '1':
            request.session['userid'] = userid
            return HttpResponseRedirect(reverse('cr_index'))
    return render(request, 'chinarailway/login.html', context = {'userid':userid})

def user_logout(request):
    request.session['userid'] = None
    return HttpResponseRedirect(reverse('cr_index'))