from django.conf.urls.defaults import *
from models import Jam

info_dict = {
    'queryset': Jam.objects.all().order_by('-date_announced'),
    'template_name': 'home.html',
}

urlpatterns = patterns('django.views.generic.list_detail',
        (r'^$', 'object_list', dict(info_dict)),
)
#        (r'^(P<jam_id>\d+)/$', 'jam_detail'),
#        (r'^(P<jam_id>\d+)/(?P<entry_id>\d+)/$', 'entry_detail'),
#)
