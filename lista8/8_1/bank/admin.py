from django.contrib import admin

# Register your models here.
from django.contrib.auth.admin import UserAdmin
from django.contrib.auth.forms import UserCreationForm
from django.contrib.auth.models import User
from django.forms import forms

from bank.models import Customer, Transfer


class CustomerCreationForm(UserCreationForm):
    account_number = forms.Field(
        label="Account number",
        required=True,
    )

class CustomerInline(admin.StackedInline):
    model = Customer

class CustomerAdmin(UserAdmin):
    inlines = [ CustomerInline, ]


admin.site.unregister(User)
admin.site.register(User, CustomerAdmin)
admin.site.register(Customer)
admin.site.register(Transfer)


