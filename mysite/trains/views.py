from django.shortcuts import render
from django.shortcuts import render_to_response
from django.http import HttpResponse
from django.template import loader
# Create your views here.

answer = [
    ['G1', 'PEK', '08:00', 'SHH', '09:00', 3, 500.00, 0, 200.00, 50, 10.00],
    ['G2', 'PEK', '18:00', 'SHH', '19:00', 3, 500.00, 10, 200.00, 50, 10.00]
]
context = {'login_name':'test', 'authority':1, 'asked': True, 'Trains': answer}


def index(request):
    return render_to_response("SeekTrain.html", context)

def addTrain(request):
    return render_to_response("add_train.html", context)
