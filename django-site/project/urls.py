from django.conf.urls.defaults import *
from django.conf import settings
from django.contrib.auth.models import User

# Uncomment the next two lines to enable the admin:
from django.contrib import admin
admin.autodiscover()

urlpatterns = patterns('',
    (r'^$', 'django.views.generic.simple.redirect_to', {'url': '/jams/'}),
    (r'^jams/', include('gamejam.jam.urls')),
    #(r'^accounts/', include('registration.backends.default.urls')),
    url(r'^accounts/register/$',
        'registration.views.register',
        {
            'backend': 'registration.backends.simple.SimpleBackend',
            'success_url': '/accounts/profile/',
        },
        name='registration_register'),
    (r'^accounts/', include('registration.backends.simple.urls')),
    (r'^accounts/login/$', 'django.contrib.auth.views.login'),
    (r'^accounts/logout/$', 'django.contrib.auth.views.logout'),
    (r'^accounts/profile/$', 'django.views.generic.simple.direct_to_template', {
        'template': 'auth/profile.html'
    }),
    # Uncomment the admin/doc line below to enable admin documentation:
    (r'^admin/doc/', include('django.contrib.admindocs.urls')),

    # Uncomment the next line to enable the admin:
    (r'^admin/', include(admin.site.urls)),
)

if settings.DEBUG:
    print settings.MEDIA_ROOT
    urlpatterns += patterns('',
        (r'^static/(?P<path>.*)$', 'django.views.static.serve', {'document_root': settings.MEDIA_ROOT}),
    )
