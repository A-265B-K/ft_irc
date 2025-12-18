#!/bin/bash
cd src
ls -pR | grep *.cpp | column > ../src.list
