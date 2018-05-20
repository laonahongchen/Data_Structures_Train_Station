from django.shortcuts import render
from django.shortcuts import render_to_response
from django.http import HttpResponse
from django.template import loader
# Create your views here.

import os
import ctypes

context = {'login_name':'test', 'authority':2}
context1 = {'login_name':'test', 'authority':2, 'num_price':range(1)}

trains = ['商务座']

station_info = [
    ["北京", "xx:xx" ,"08:00","00:00", [0.0]],
    ["夏威夷", "08:02", "xx:xx", "00:00" , [1.5]],
]

context2 = {'login_name':'test', 'authority':2, 'style':1, 'num_price':range(1), 'num_station':range(2), 'class_train':trains}

context3 = {'login_name':'test', 'authority':2, 'style':1, 'num_price':range(1), 'class_train':trains, 'station': station_info, 'asked':False, 'has_train':False}

def getServerSideCookie(request, cookie, default_val=None):
    val = request.session.get(cookie)
    if not val:
        val = default_val
    return val

def index(request):
    userid = getServerSideCookie(request, 'userid', '0')
    userpv = getServerSideCookie(request, 'userpv', '0')

    return render(request, 'Add_train.html', context = {'login_name':userid, 'authority':userpv})

def index1(request):
    return render_to_response("Add_train_in_class.html", context1)

def index2(request):
    return render_to_response("Add_train_in_station.html", context2)

def query_train(request):
    userid = getServerSideCookie(request, 'userid', '0')
    userpv = getServerSideCookie(request, 'userpv', '0')

    return render(request, 'AskTrain.html', context = {'login_name':userid, 'authority':userpv})
