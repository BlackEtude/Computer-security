import json
import urllib
from pprint import pformat

import requests
import sys
from django.contrib import auth, messages
from django.http import *
from django.shortcuts import redirect, render
from django.contrib.auth.decorators import login_required
from django_tables2 import RequestConfig

from bank.models import Transfer, Customer
from bank.tables import HistoryTable
from lista4 import settings


def login(request):
    if request.user.is_authenticated():
        sys.stderr.write("user already authenticated\n")
        return redirect('homepage')
    if request.method == 'POST':
        ''' Begin reCAPTCHA validation '''
        recaptcha_response = request.POST.get('g-recaptcha-response')
        data = {
            'secret': settings.GOOGLE_RECAPTCHA_SECRET_KEY,
            'response': recaptcha_response
        }
        r = requests.post('https://www.google.com/recaptcha/api/siteverify', data=data)
        result = r.json()
        ''' End reCAPTCHA validation '''

        sys.stderr.write(result.__str__())

        if result['success']:
            sys.stderr.write("captcha ok\n")
            username = request.POST.get('username')
            password = request.POST.get('password')
            user = auth.authenticate(username=username, password=password)

            if user is not None:
                # correct username and password login the user
                auth.login(request, user)
                # return HttpResponse(request)
                return redirect('homepage')

            else:
                messages.error(request, 'Error wrong username/password')
                return render(request, 'login.html')
        else:
            sys.stderr.write("captcha failed\n")
            messages.error(request, 'Invalid reCAPTCHA. Please try again.')
            return render(request, 'login.html')

    return render(request, 'login.html')


@login_required(login_url='/login/')
def transfer(request):
    # sys.stderr.write("funkcja transfer")
    if request.method == 'POST':
        recipient = request.POST.get('recipient_name')
        account = request.POST.get('recipient_account')
        amount = request.POST.get('amount')

        c = {"recipient_name": recipient, "recipient_account": account, "amount": amount}

        if request.POST.get('confirm') is not None:
            # sys.stderr.write("funkcja transfer confirm")
            t = Transfer.objects.create(sender=request.user,
                                        receiver_name=recipient,
                                        receiver_account=account,
                                        amount=amount)
            return render(request, 'transfer_confirmation_server.html', c)

        if request.POST.get('server') is not None:
            # sys.stderr.write("funkcja transfer confirm serwer\n")
            return redirect('homepage')

        return render(request, 'transfer_confirmation.html', c)
    return render(request, 'transfer_form.html')



def logout(request):
    auth.logout(request)
    return render(request,'logout.html')


@login_required(login_url='/login/')
def homepage(request):
    if not request.user.is_authenticated():
        return redirect('login')
    # num = Customer.objects.filter(user_id=request.user.id).values('account_number')[0]['account_number']
    account = Customer.objects.filter(user_id=request.user.id).values('account_number')
    sent = HistoryTable(Transfer.objects.filter(sender=request.user))
    received = HistoryTable(Transfer.objects.filter(receiver_account=account))
    RequestConfig(request).configure(sent)
    RequestConfig(request).configure(received)
    return render(request, 'homepage.html', {'sent': sent, 'received': received})


@login_required(login_url='/login/')
def history(request):
    account = Customer.objects.filter(user_id=request.user.id).values('account_number')
    sent = HistoryTable(Transfer.objects.filter(sender=request.user))
    received = HistoryTable(Transfer.objects.filter(receiver_account=account))
    RequestConfig(request).configure(sent)
    RequestConfig(request).configure(received)
    return render(request, 'history.html', {'sent': sent, 'received': received})

def main(request):
    return redirect('/login/')


def get(self, request, **kwargs):
    ctx = dict(
        user_dn=request.META[
            'HTTP_X_SSL_USER_DN'],
        authentication_status=request.META['HTTP_X_SSL_AUTHENTICATED'],
        user=str(request.user))
    return HttpResponse(pformat(ctx), content_type="text/plain")

