# Running as admin
check if you are running with administrator's privilege

# Build
launch the "x64 Native Tools Command Prompt for VS 2022"

``` console
cd running-as-admin
build
.\build\running_as_admin.exe
```

will print `YES` and retun 1 if running as admin, `NO` and 0 otherwise.

check the return value with:
``` console
echo %ERRORLEVEL%
```
