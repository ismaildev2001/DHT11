from rest_framework.decorators import api_view
from rest_framework.response import Response
from django.http import HttpResponse

def coucou(request):
    return HttpResponse("Hello Zaid")