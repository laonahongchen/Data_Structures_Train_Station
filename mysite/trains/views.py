from django.shortcuts import render
from django.shortcuts import render_to_response
from django.http import HttpResponse
from django.template import loader
# Create your views here.


context = {'login_name':'test', 'authority':1, 'asked': True}

def index(request):
    return render_to_response("SeekTrain.html", context)
