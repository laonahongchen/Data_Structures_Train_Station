from django.shortcuts import render
from django.shortcuts import render_to_response
from django.http import HttpResponse
from django.template import loader
from django.template import RequestContext

answer = [
    ['G1', 'T', 'PEK', '08:00', 'SHH', '10:00', [["一等座", 3, 500.00], ["二等座", 2, 200.00], ["三等座", 1, 50.00]]],
    ['G2', 'G', 'PEK', '18:00', 'SHH', '19:00', [['一等座', 0, 500.00], ['二等座', 0, 200.00], ['三等座', 1, 50.00]]],
]

answer1 = [
    ['G1', 'T', 'PEK', '08:00', 'SHH', '10:00', [["一等座", 3, 500.00], ["二等座", 0, 200.00], ["三等座", 1, 50.00]]],
    ['G2', 'G', 'PEK', '18:00', 'SHH', '19:00', [['一等座', 0, 500.00], ['二等座', 0, 200.00], ['三等座', 0, 50.00]]],
]

catalogs = {'C-城际铁路', 'G-高铁', 'K-快车'}

context = {'login_name':'test', 'authority':1, 'style':1, 'asked': True, 'Trains': answer, 'catalogs': catalogs}
context1 = {'login_name':'test', 'authority':1, 'style':1, 'asked': True, 'Historys': answer1, 'catalogs': catalogs}

def getServerSideCookie(request, cookie, default_val=None):
    val = request.session.get(cookie)
    if not val:
        val = default_val
    return val


def index(request):
    userid = getServerSideCookie(request, 'userid', '0')
    userpv = getServerSideCookie(request, 'userpv', '0')

    context['login_name'] = userid
    context['authority'] = userpv
    context['style'] = getServerSideCookie(request, 'tmpstyle', '1')

    if request.method == 'POST':
        fs = request.POST.get('from')
        ts = request.POST.get('to')
        date = request.POST.get('date')
        context['fs'] = fs
        context['ts'] = ts
        context['date'] = date
        return render(request, 'SeekTickets.html', context)

    return render(request, 'SeekTickets.html', context)
'''
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
'''

'''
    if request.method == 'GET':
        #asked = request.COOKIES.get('asked')
        asked = False
        if asked != True:
            context['asked'] = False
        else:
            context['asked'] = True
        return render(request, "SeekTickets.html", context)
    else:
        context['asked'] = True
        fr = request.POST.get('from')
        des = request.POST.get('destination')
        dat = request.POST.get('dateoftrain')
        return render(request, "SeekTickets.html", context)
'''
def buy_history(request):
    userid = getServerSideCookie(request, 'userid', '0')
    userpv = getServerSideCookie(request, 'userpv', '0')
    
    context['login_name'] = userid
    context['authority'] = userpv
    context['style'] = getServerSideCookie(request, 'tmpstyle', '1')

    return render_to_response("Buyhistory.html", context)
