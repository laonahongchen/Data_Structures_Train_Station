from django.shortcuts import render
from django.shortcuts import render_to_response
from django.http import HttpResponse
from django.template import loader
# Create your views here.

context = {'login_name':'test', 'authority':2}
context1 = {'login_name':'test', 'authority':2, 'num_price':range(1)}

trains = ['商务座']

station_info = [
    ["北京", "xx:xx" ,"08:00","00:00", [0.0]],
    ["夏威夷", "08:02", "xx:xx", "00:00" , [1.5]],
]

context2 = {'login_name':'test', 'authority':2, 'num_price':range(1), 'num_station':range(2), 'class_train':trains}

context3 = {'login_name':'test', 'authority':2, 'num_price':range(1), 'class_train':trains, 'station': station_info, 'asked':False, 'has_train':False}

def index(request):
    return render_to_response("add_train.html", context)

def index1(request):
    return render_to_response("add_train_in_class.html", context1)

def index2(request):
    return render_to_response("add_train_in_station.html", context2)

def query_train(request):
    return render_to_response("AskTrain.html", context3)
