#!/bin/bash
find src -type f -printf '%f ' | column > src.list
