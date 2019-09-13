#! /bin/bash

source testing.sh

for folder in tests/*; do
  source $folder/script.sh
done
