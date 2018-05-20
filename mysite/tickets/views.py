from django.shortcuts import render
from django.shortcuts import render_to_response
from django.http import HttpResponse
from django.template import loader
from django.template import RequestContext

answer = [
    ['G1', 'PEK', '08:00', 'SHH', '10:00', [["一等座", 3, 500.00], ["二等座", 2, 200.00], ["三等座", 1, 50.00]]],
    ['G2', 'PEK', '18:00', 'SHH', '19:00', [['一等座', 0, 500.00], ['二等座', 0, 200.00], ['三等座', 1, 50.00]]],
]

answer1 = [
    ['G1', 'PEK', '08:00', 'SHH', '10:00', [["一等座", 3, 500.00], ["二等座", 0, 200.00], ["三等座", 1, 50.00]]],
    ['G2', 'PEK', '18:00', 'SHH', '19:00', [['一等座', 0, 500.00], ['二等座', 0, 200.00], ['三等座', 0, 50.00]]],
]

catalogs = {'C-城际铁路', 'G-高铁', 'K-快车'}

context = {'login_name':'test', 'authority':1, 'style':1, 'asked': True, 'Trains': answer, 'catalogs': catalogs}
context1 = {'login_name':'test', 'authority':1, 'style':1, 'asked': True, 'Historys': answer1, 'catalogs': catalogs}

def getServerSideCookie(request, cookie, default_val=None):
    val = request.session.get(cookie)
    if not val:
        val = default_val
    return val

# Create your views here.
def index(request):
    userid = getServerSideCookie(request, 'userid', '0')
    userpv = getServerSideCookie(request, 'userpv', '0')

    if request.method == 'GET':
        #asked = request.COOKIES.get('asked')
        asked = False
        if asked != True:
            context['asked'] = False
        else:
            context['asked'] = True
        return render(request, "SeekTickets.html", context = {'login_name':userid, 'authority':userpv})
    else:
        context['asked'] = True
        fr = request.POST.get('from')
        des = request.POST.get('destination')
        dat = request.POST.get('dateoftrain')
        return render(request, "SeekTickets.html", context = {'login_name':userid, 'authority':userpv})

def buy_history(request):
    userid = getServerSideCookie(request, 'userid', '0')
    userpv = getServerSideCookie(request, 'userpv', '0')

    return render_to_response("Buyhistory.html", context = {'login_name':userid, 'authority':userpv})
