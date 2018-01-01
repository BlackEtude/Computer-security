# -*- coding: utf-8 -*-
# Generated by Django 1.11.7 on 2017-11-18 17:06
from __future__ import unicode_literals

from django.db import migrations, models
import django.utils.timezone


class Migration(migrations.Migration):

    dependencies = [
        ('bank', '0004_auto_20171118_1635'),
    ]

    operations = [
        migrations.RenameField(
            model_name='transfer',
            old_name='receiver',
            new_name='receiver_account',
        ),
        migrations.AddField(
            model_name='transfer',
            name='receiver_name',
            field=models.TextField(default=django.utils.timezone.now),
            preserve_default=False,
        ),
        migrations.AlterField(
            model_name='customer',
            name='account_number',
            field=models.PositiveIntegerField(default=6438, unique=True),
        ),
    ]
