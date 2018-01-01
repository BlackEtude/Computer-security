import random

from django.contrib.auth.models import User
from django.db import models
from django.utils import timezone


class Customer(models.Model):
    user = models.OneToOneField(User, null=True, blank=True)
    account_number = models.PositiveIntegerField(null=False, default=random.randint(1000, 9999), unique=True)

    def __str__(self):
        return self.user.username


class Transfer(models.Model):
    sender = models.ForeignKey(User)
    receiver_name = models.TextField(null=False)
    receiver_account = models.PositiveIntegerField(null=False)
    amount = models.DecimalField(null=False, decimal_places=2, max_digits=10)
    date = models.DateTimeField(default=timezone.now)


