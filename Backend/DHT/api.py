from .models import Dht11
from .serializers import DHT11Serializer
from rest_framework.decorators import api_view
from rest_framework import generics
from rest_framework.response import Response


@api_view(["GET"])
def Dlist(request):
    all_data = Dht11.objects.all()
    data = DHT11Serializer(all_data, many=True).data
    return Response({"data": data})


class Dhtviews(generics.CreateAPIView):
    queryset = Dht11.objects.all()
    serializer_class = DHT11Serializer