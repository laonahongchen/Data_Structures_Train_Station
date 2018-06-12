from django.shortcuts import render
from django.shortcuts import render_to_response
from django.http import HttpResponse, HttpResponseRedirect
from django.urls import reverse
from django.template import loader
from django.template import RequestContext

import os 
import ctypes

def getServerSideCookie(request, cookie, default_val=None):
    val = request.session.get(cookie)
    if not val:
        val = default_val
    return val

def index(request):
    context = {}
    context['login_name'] = getServerSideCookie(request, 'userid', '0')
    context['authority'] = getServerSideCookie(request, 'userpv', '0')
    context['style'] = getServerSideCookie(request, 'tmpstyle', '1')

    if request.method == 'POST':
        trainid = request.POST.get('trainid')
        if trainid != None:
            userid = context['login_name']
            num_buy = request.POST.get('num_buy')
            fr = request.POST.get('trainfr')
            to = request.POST.get('trainto')
            date = request.POST.get('date')
            class_name = request.POST.get('class_name')

            lib = ctypes.cdll.LoadLibrary('./lib/crsystem/libcr.so')
            dataInput = ctypes.create_string_buffer(' '.join((userid, num_buy, trainid, fr, to, date, class_name)).encode('UTF-8'))
            dataOutput = ctypes.create_string_buffer(10)
            inputPointer = (ctypes.c_char_p)(ctypes.addressof(dataInput))
            outputPointer = (ctypes.c_char_p)(ctypes.addressof(dataOutput))
            lib.buyTicket(inputPointer, outputPointer)
            info = dataOutput.value.decode('UTF-8')

            return HttpResponseRedirect(reverse('buy_history'))

        context['asked'] = True 

        fs = request.POST.get('from')
        ts = request.POST.get('to')
        date = request.POST.get('date')
        context['fs'] = fs
        context['ts'] = ts
        context['date'] = date

        lib = ctypes.cdll.LoadLibrary('./lib/crsystem/libcr.so')
        dataInput = ctypes.create_string_buffer(' '.join((fs, ts, date, 'GCDZTK')).encode('UTF-8'))
        dataOutput = ctypes.create_string_buffer(50000)
        inputPointer = (ctypes.c_char_p)(ctypes.addressof(dataInput))
        outputPointer = (ctypes.c_char_p)(ctypes.addressof(dataOutput))
        lib.queryTicket(inputPointer, outputPointer)
        info = dataOutput.value.decode('UTF-8')

        print(info)

        if info == '0':
            return render(request, 'SeekTickets.html', context)

        Trains = []
        for item in info.split('|'):
            ticket = item.split()
            x = []
            x.append(ticket[0])
            x.append(ticket[1])
            x.append(ticket[2])
            x.append(ticket[4])
            x.append(ticket[5])
            x.append(ticket[7])
            class_price = []
            p = 8
            while 1:
                if p == len(ticket):
                    break
                price = []
                price.append(ticket[p])
                price.append(ticket[p + 1])
                price.append(str(round(float(ticket[p + 2]))))
                p += 3
                class_price.append(price)
            x.append(class_price)
            Trains.append(x)
        print(Trains)
        context['Trains'] = Trains

    return render(request, 'SeekTickets.html', context)

def buy_history(request):
    context = {}

    context['login_name'] = getServerSideCookie(request, 'userid', '0')
    context['authority'] = getServerSideCookie(request, 'userpv', '0')
    context['style'] = getServerSideCookie(request, 'tmpstyle', '1')

    if request.method == 'POST':
        userid = context['login_name']
        date = request.POST.get('date')

        lib = ctypes.cdll.LoadLibrary('./lib/crsystem/libcr.so')
        dataInput = ctypes.create_string_buffer(' '.join((userid, date, 'GCDZTK')).encode('UTF-8'))
        dataOutput = ctypes.create_string_buffer(50000)
        inputPointer = (ctypes.c_char_p)(ctypes.addressof(dataInput))
        outputPointer = (ctypes.c_char_p)(ctypes.addressof(dataOutput))
        lib.queryOrder(inputPointer, outputPointer)
        info = dataOutput.value.decode('UTF-8')

        if info == '0':
            return render(request, "Buyhistory.html", context)

        print(info)

        Historys = []
        for item in info.split('|'):
            ticket = item.split()
            x = []
            x.append(ticket[0])
            x.append(ticket[1])
            x.append(ticket[2])
            x.append(ticket[4])
            x.append(ticket[5])
            x.append(ticket[7])
            class_price = []
            p = 8
            while 1:
                if p == len(ticket):
                    break
                price = []
                price.append(ticket[p])
                price.append(ticket[p + 1])
                price.append(str(round(float(ticket[p + 2]))))
                p += 3
                class_price.append(price)
            x.append(class_price)
            Historys.append(x)
        print(Historys)
        context['Historys'] = Historys

    return render(request, "Buyhistory.html", context)
