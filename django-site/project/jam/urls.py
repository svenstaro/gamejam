from django.conf.urls.defaults import *
from models import *

urlpatterns = patterns('',
        (r'^$', 'django.views.generic.list_detail.object_list', {
            'queryset': Jam.objects.all().order_by('-date_announced'),
            'template_name': 'home.html',
        }),
        (r'^(?P<jam_id>\d+)/$', 'gamejam.jam.views.jam_detail'),
        (r'^entry/(?P<entry_id>\d+)/$', 'gamejam.jam.views.entry_detail'),
        (r'^voting-finished/$', 'django.views.generic.simple.direct_to_template', {
            'template': 'jam/voting_finished.html'
        }),

)
