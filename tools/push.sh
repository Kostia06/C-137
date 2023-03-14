#!/bin/bash

if [ -z "$1" ]; then
    echo "First argument is empty please add name for push"
    exit 1
fi

cd ../ && 
git add . &&
git commit -m "$1" &&
git brach -M main &&
git push -u origin main