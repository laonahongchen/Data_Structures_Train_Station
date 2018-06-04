from django.shortcuts import render
from django.shortcuts import render_to_response
from django.http import HttpResponse,HttpResponseRedirect
from django.template import loader
from django.urls import reverse

from django.conf import settings
from django.contrib import messages
# Create your views here.

import os
import ctypes
import requests

context = {'login_name':'test', 'authority':0, 'style':'1'}
#need to fix context which send login_name and authority to html
#authority 0: not login 1: normal user 2:admin

def getServerSideCookie(request, cookie, default_val=None):
    val = request.session.get(cookie)
    if not val:
        val = default_val
    return val

def cstyle0(request):
    request.session['tmpstyle'] = '0'
    return HttpResponseRedirect(reverse('index'))

def cstyle1(request):
    request.session['tmpstyle'] = '1'
    return HttpResponseRedirect(reverse('index'))

def index(request):
    userid = getServerSideCookie(request, 'userid', '0')
    userpv = getServerSideCookie(request, 'userpv', '0')

    context['login_name'] = userid
    context['authority'] = userpv
    context['style'] = getServerSideCookie(request, 'tmpstyle', '1')

    return render(request, 'index.html', context)

def about(request):
    userid = getServerSideCookie(request, 'userid', '0')
    userpv = getServerSideCookie(request, 'userpv', '0')

    context['login_name'] = userid
    context['authority'] = userpv
    context['style'] = getServerSideCookie(request, 'tmpstyle', '1')

    return render(request, 'About.html', context)

def uploading(request):
    userid = getServerSideCookie(request, 'userid', '0')
    userpv = getServerSideCookie(request, 'userpv', '0')

    context['login_name'] = userid
    context['authority'] = userpv
    context['style'] = getServerSideCookie(request, 'tmpstyle', '1')

    return render(request, 'Uploading.html', context)

def login(request):
    userid = getServerSideCookie(request, 'userid', '0')
    userpv = getServerSideCookie(request, 'userpv', '0')
    if userid != '0':
        return HttpResponseRedirect(reverse('index'))

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
            # I should query its privilege, but set to superroot for now
            request.session['userpv'] = '2'
            return HttpResponseRedirect(reverse('index'))

    context['login_name'] = userid
    context['authority'] = userpv
    context['style'] = getServerSideCookie(request, 'tmpstyle', '1')

    return render(request, 'Login.html', context)

def signup(request):
    userid = getServerSideCookie(request, 'userid', '0')
    userpv = getServerSideCookie(request, 'userpv', '0')

    if userid != '0':
        return HttpResponseRedirect(reverse('index'))

    if request.method == 'POST':
        username = request.POST.get('username')
        emailaddress = request.POST.get('emailaddress')
        phonenumber = request.POST.get('phonenumber')
        password = request.POST.get('password')

        recaptcha_response = request.POST.get('g-recaptcha-response')
        data = {
            'secret': settings.GOOGLE_RECAPTCHA_SECRET_KEY,
            'response': recaptcha_response
        }

        #r = requests.post('https://www.google.com/recaptcha/api/siteverify', data=data)
        #result = r.json()

    #    if result['success']:
        lib = ctypes.cdll.LoadLibrary('./lib/crsystem/libcr.so')
        dataInput = ctypes.create_string_buffer(' '.join((username, password, emailaddress, phonenumber)).encode('UTF-8'), 1000)
        dataOutput = ctypes.create_string_buffer(1000)
        inputPointer = (ctypes.c_char_p)(ctypes.addressof(dataInput))
        outputPointer = (ctypes.c_char_p)(ctypes.addressof(dataOutput))
        lib.userRegister(inputPointer, outputPointer)
        info = dataOutput.value.decode('UTF-8')

        if info != '-1':
            return HttpResponseRedirect(reverse('index'))
    #    else:
    #         messages.error(request, 'Invalid reCAPTCHA. Please try again.')
    context['login_name'] = userid
    context['authority'] = userpv
    context['style'] = getServerSideCookie(request, 'tmpstyle', '1')

    return render(request, 'Signup.html', context)

def user_logout(request):
    request.session['userid'] = None
    request.session['userpv'] = None
    request.session['tmpstyle'] = None
    return HttpResponseRedirect(reverse('index'))

def cinfo(request):
    return render_to_response('Signup.html', context)

def page_not_found(request):
    userid = getServerSideCookie(request, 'userid', '0')
    userpv = getServerSideCookie(request, 'userpv', '0')

    context['login_name'] = userid
    context['authority'] = userpv
    context['style'] = getServerSideCookie(request, 'tmpstyle', '1')

    return render(request, 'Error.html', context)

def page_error(request):
    userid = getServerSideCookie(request, 'userid', '0')
    userpv = getServerSideCookie(request, 'userpv', '0')

    context['login_name'] = userid
    context['authority'] = userpv
    context['style'] = getServerSideCookie(request, 'tmpstyle', '1')

    return render(request, 'Error.html', context)
