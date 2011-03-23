from django.conf.urls.defaults import *
from django.conf import settings
from django.contrib.auth.models import User
from registration.views import register

# Uncomment the next two lines to enable the admin:
from django.contrib import admin
admin.autodiscover()

urlpatterns = patterns('',
    (r'^$', 'django.views.generic.simple.redirect_to', {'url': '/jams/'}),
    (r'^jams/', include('gamejam.jam.urls')),

    url(r'^accounts/register/$',
        register,
        {'backend': 'gamejam.jam.registration_backend.SimpleUnactiveBackend'},
        name='registration_register'),
    url(r'^accounts/register/closed/$',
        'django.views.generic.simple.direct_to_template',
        {'template': 'registration/registration_closed.html'},
        name='registration_disallowed'),
    (r'^accounts/login/$', 'django.contrib.auth.views.login'),
    (r'^accounts/logout/$', 'django.contrib.auth.views.logout'),
    (r'^accounts/profile/$', 'django.views.generic.simple.direct_to_template', {
        'template': 'auth/profile.html'
    }),
    (r'^accounts/uuid-auth/(?P<uuid>[0-9a-f]+)/$', 'django.views.generic.simple.direct_to_template', {
        'template': 'registration/uuid.html'
    }),
    (r'^accounts/', include('registration.auth_urls')),

    (r'^admin/doc/', include('django.contrib.admindocs.urls')),
    (r'^admin/', include(admin.site.urls)),
)

if settings.DEBUG:
    print settings.MEDIA_ROOT
    urlpatterns += patterns('',
        (r'^static/(?P<path>.*)$', 'django.views.static.serve', {'document_root': settings.MEDIA_ROOT}),
    )
