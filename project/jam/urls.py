from django.conf.urls.defaults import *

urlpatterns = patterns('django.views.generic.simple',
        (r'^$', 'direct_to_template', {'template': 'base.html'}),
)
