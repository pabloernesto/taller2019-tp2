#! /bin/bash

# Usage: pass
function pass() {
  echo "$folder passed"
  $cleanup
}

# Usage: fail [msg]
function fail() {
  echo "$folder failed:"
  if [ -n "$1" ]; then echo -e "\t$1"; fi
  $cleanup
  exit
}
