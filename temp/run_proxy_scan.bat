
@echo off
python ..\..\rapidproxyscan-v-0-4-0.py --single-run
copy /Y working_proxies_*.txt ..\data\working_proxies.txt >nul
