#!/bin/bash

if [ -z "$1" ]; then
    echo "First argument is empty"
    exit 1
fi

git reset --soft HEAD~$1
git push origin main --force
