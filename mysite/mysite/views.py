from django.shortcuts import render
from django.shortcuts import render_to_response
from django.http import HttpResponse
from django.template import loader
# Create your views here.

context = {'login_name':'test', 'authority':1}

def index(request):
    return render(request, 'index.html', context)

def about(request):
    return render_to_response('about.html', context)

def login(request):
    return render_to_response('login.html', context)
