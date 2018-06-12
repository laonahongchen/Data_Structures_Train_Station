from django.shortcuts import render
from django.shortcuts import render_to_response
from django.http import HttpResponse, HttpResponseRedirect
from django.urls import reverse
from django.template import loader
# Create your views here.

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
        trainname = request.POST.get('trainname')
        catalogs = request.POST.get('catalogs')
        num_station = request.POST.get('num_station')
        num_price = request.POST.get('num_price')

        request.session['trainid'] = trainid
        request.session['trainname'] = trainname
        request.session['catalogs'] = catalogs
        request.session['num_station'] = num_station
        request.session['num_price'] = num_price

        return HttpResponseRedirect(reverse('train_add_1'))

    return render(request, 'Add_train.html', context)

def index1(request):
    context = {}
    context['login_name'] = getServerSideCookie(request, 'userid', '0')
    context['authority'] = getServerSideCookie(request, 'userpv', '0')
    context['style'] = getServerSideCookie(request, 'tmpstyle', '1')
    num_price = int(getServerSideCookie(request, 'num_price', '0'))
    context['num_price'] = range(num_price)

    if request.method == 'POST':
        class_train = []
        for i in range(num_price):
            x = request.POST.get('class_train[' + str(i) + ']')
            class_train.append(x)
        request.session['class_train'] = class_train

        return HttpResponseRedirect(reverse('train_add_2'))

    return render(request, "Add_train_in_class.html", context)

def timeFormat(s):
    if len(s) == 1:
        s = '0' + s
    return s

def getGapTime(s, t):
    a = list(map(int, s.split(':')))
    b = list(map(int, t.split(':')))
    h = b[0] - a[0]
    m = b[1] - a[1]
    if (m < 0):
        m += 60
        h -= 1
    return timeFormat(str(h)) + ':' + timeFormat(str(m))

def index2(request):
    context = {}
    context['login_name'] = getServerSideCookie(request, 'userid', '0')
    context['authority'] = getServerSideCookie(request, 'userpv', '0')
    context['style'] = getServerSideCookie(request, 'tmpstyle', '1')

    num_price = int(getServerSideCookie(request, 'num_price', '0'))
    num_station = int(getServerSideCookie(request, 'num_station', '0'))
    class_train = getServerSideCookie(request, 'class_train')

    context['num_price'] = range(num_price)
    context['num_station'] = range(num_station)
    context['class_train'] = class_train

    if request.method == 'POST':
        name_station = []
        time_arriv = []
        time_start = []
        time_stop = []
        sta_price = []
        for i in range(num_station):
            x = request.POST.get('name_station[' + str(i) + ']')
            name_station.append(x)
            x = request.POST.get('time_arriv[' + str(i) + ']')
            time_arriv.append(x)
            x = request.POST.get('time_start[' + str(i) + ']')
            time_start.append(x)
            if i in range(1, num_station - 1):
                x = getGapTime(time_arriv[i], time_start[i])
                time_stop.append(x)
            else:
                time_stop.append('xx:xx')

            price = []
            for j in range(num_price):
                y = '￥' + request.POST.get('price[' + str(i) + '][' + str(j) + ']')
                price.append(y)
            sta_price.append(price)
        
        time_arriv[0] = 'xx:xx'

        trainid = getServerSideCookie(request, 'trainid')
        trainname = getServerSideCookie(request, 'trainname')
        catalogs = getServerSideCookie(request, 'catalogs')

        request.session['trainid'] = None 
        request.session['trainname'] = None 
        request.session['catalogs'] = None
        request.session['num_station'] = None
        request.session['num_price'] = None
        request.session['class_train'] = None

        print(num_station)

        cmd = ' '.join((trainid, trainname, catalogs, str(num_station), str(num_price), ' '.join(class_train)))
        for i in range(num_station):
            cmd = ' '.join((cmd, name_station[i], time_arriv[i], time_start[i], time_stop[i], ' '.join(sta_price[i])))
        print(cmd)

        lib = ctypes.cdll.LoadLibrary('./lib/crsystem/libcr.so')
        dataInput = ctypes.create_string_buffer(cmd.encode('UTF-8'))
        dataOutput = ctypes.create_string_buffer(10)
        inputPointer = (ctypes.c_char_p)(ctypes.addressof(dataInput))
        outputPointer = (ctypes.c_char_p)(ctypes.addressof(dataOutput))
        lib.addTrain(inputPointer, outputPointer)
        info = dataOutput.value.decode('UTF-8') 
        print(info)

        return HttpResponseRedirect(reverse('index'))

    return render(request, "Add_train_in_station.html", context)

def query_train(request):
    context = {}
    context['login_name'] = getServerSideCookie(request, 'userid', '0')
    context['authority'] = getServerSideCookie(request, 'userpv', '0')
    context['style'] = getServerSideCookie(request, 'tmpstyle', '1')

    #print(context)

    if request.method == 'POST':
        trainid = request.POST.get('trainid')

        pubtrainid = request.POST.get('pubtrainid')
        if pubtrainid != None:
            lib = ctypes.cdll.LoadLibrary('./lib/crsystem/libcr.so')
            dataInput = ctypes.create_string_buffer(pubtrainid.encode('UTF-8'))
            dataOutput = ctypes.create_string_buffer(10)
            inputPointer = (ctypes.c_char_p)(ctypes.addressof(dataInput))
            outputPointer = (ctypes.c_char_p)(ctypes.addressof(dataOutput))
            lib.saleTrain(inputPointer, outputPointer)
            info = dataOutput.value.decode('UTF-8') 
            return HttpResponseRedirect(reverse('qt'))

        deltrainid = request.POST.get('deltrainid')
        if deltrainid != None:
            lib = ctypes.cdll.LoadLibrary('./lib/crsystem/libcr.so')
            dataInput = ctypes.create_string_buffer(deltrainid.encode('UTF-8'))
            dataOutput = ctypes.create_string_buffer(10)
            inputPointer = (ctypes.c_char_p)(ctypes.addressof(dataInput))
            outputPointer = (ctypes.c_char_p)(ctypes.addressof(dataOutput))
            lib.deleteTrain(inputPointer, outputPointer)
            info = dataOutput.value.decode('UTF-8') 
            return HttpResponseRedirect(reverse('qt'))

        print(trainid)

        lib = ctypes.cdll.LoadLibrary('./lib/crsystem/libcr.so')
        dataInput = ctypes.create_string_buffer(trainid.encode('UTF-8'))
        dataOutput = ctypes.create_string_buffer(50000)
        inputPointer = (ctypes.c_char_p)(ctypes.addressof(dataInput))
        outputPointer = (ctypes.c_char_p)(ctypes.addressof(dataOutput))
        lib.queryTrain(inputPointer, outputPointer)
        info = dataOutput.value.decode('UTF-8') 
        #print(info)

        context['asked'] = True

        if info == '0':
            context['has_train'] = False
        else:
            context['has_train'] = True

            data = info.split()

            context['train_id'] = data[0]
            context['train_name'] = data[1]
            context['catalogs'] = data[2]

            num_station = data[3]
            num_price = data[4]

            class_train = []
            for i in range(5, 5 + int(num_price)):
                class_train.append(data[i])

            p = 5 + int(num_price)
            station = []
            for i in range(0, int(num_station)):
                x = []
                for j in range(0, 4):
                    x.append(data[p])
                    p += 1
                price = []
                for j in range(0, int(num_price)):
                    price.append(str(round(float(data[p]))))
                    p += 1
                x.append(price)
                station.append(x)

            context['class_train'] = class_train
            context['station'] = station

            #print(station)

        return render(request, 'AskTrain.html', context) 

    return render(request, 'AskTrain.html', context)
