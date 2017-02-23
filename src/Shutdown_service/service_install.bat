sc create shutdownservice binPath="%~dp0Shutdown_service.exe"
sc config shutdownservice start=auto
sc start shutdownservice