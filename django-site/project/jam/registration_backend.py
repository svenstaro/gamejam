import registration
from django.contrib.auth.models import User
from registration.backends.simple import SimpleBackend
from registration import signals

class SimpleUnactiveBackend(SimpleBackend):
    def register(self, request, **kwargs):
        username, email, password = kwargs['username'], kwargs['email'], kwargs['password1']
        new_user = User.objects.create_user(username, email, password)
        new_user.is_active = False
        new_user.save()

        signals.user_registered.send(sender=self.__class__,
                                     user=new_user,
                                     request=request)
        return new_user

    def post_registration_redirect(self, request, user):
        """
        After registration, redirect to the user's account page.
        
        """
        return ('/accounts/uuid-auth/%s/' % user.get_profile().uuid, (), {})
