
## How to build and deploy

Click ADD and add the project to Unity HUB

Set correct Unity version for the project

![Unity version](./unityversion.png)

Sett Build Settings ved å gå i File->Build Settings. Ved Plattform "Android" og klikk "Switch Platform"



## Troubleshooting

Hvis du ikke klarer å laste opp ny APK, kan du prøve å avinstallere gammel versjon manuelt:

```adb uninstall <app>```

Eksempel:

``` /Users/badeand/Library/Android/sdk/platform-tools/adb uninstall com.hwguild.labyrinth```

