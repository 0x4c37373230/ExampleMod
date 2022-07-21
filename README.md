# <img src="https://cdn.discordapp.com/attachments/853624220306898974/973669248666828860/RTPLogo.png" width="70" height="70"/> RunTime Pistorder

This BDS mod, which I originally made while messing around with DLL modding for the first time and then adapted into an example for others to learn with a not yet released guide; aims to be a "runtime" port of the Java Edition mod, [pistorder](https://github.com/Fallen-Breath/pistorder) by Fallen-Breath; with "runtime" meaning it will not tell you what ***will happen*** like pistorder does, it'll tell you what ***happens*** when it happens. 
This is still very much a work in progress, and I'll appreciate any suggestions or ideas anyone might have. 

### Implemented Features

<details>
<summary></summary>
<br>
As of the time of writing this, the mod will log:
   <ul>
      <li>Piston updates</li>
      <li>Piston extension</li>
      <li>Piston retraction</li>
      <li>Pistons being moved</li>
  </ul> 
  As well as their types, coordinates and the amount of gameticks spent since the last event
</details>

### Planned Features

<details>
<summary></summary>
<br>
   <ul>
      <li>Logging when and why a piston was unable to extend</li>
  </ul> 
</details>

### Long Term TODO List

<details>
<summary></summary>
<br>
   <ul>
      <li>Setting to ignore pistons at the specified coordinates</li>
      <li>Piston "naming" to recognize them easily</li>
  </ul> 
</details>

## Injecting

Download and run the [Xenos](https://github.com/DarthTon/Xenos) dll injector. In the **Process selection** section; select **nNew** and open the bedrock_server.exe file. Then on the **Images** section, click on the **Add** button and select the MCMODDLL dll file; then click on the **Inject** button

<p align="center">
  <img src="https://user-images.githubusercontent.com/81709312/143133453-861be19d-86f5-4277-be48-488f8263e6cd.png" />
</p>

## plugins/RtP/RtP-cfg.txt

When injected for the first time, the mod will create a settings file in the path mentioned above as the title of this section. It'll look something like this:
```yaml
logAll: true
logArm: true
logUpdates : true
logMovement: true
deleteBlocksOnExtension : false
```
`logAll` determines if the mod should log everything, in case you're lazy and don't want to change individual settings. If `logAll` is disabled, the mod will still log individual enabled settings. The following 3 settings determine if the mod should log extension and retraction, piston updates and pistons moving or being moved respectively. The last setting determines if pistons should act as if no blocks were in front of them even if there happened to be some. I honestly have no idea why anyone would want that but I don't really care either.

## 3rd Party Code

- [MCMODDLL-CPP](https://github.com/zhkj-liuxiaohua/MCMODDLL-CPP)
- [termcolor](https://github.com/ikalnytskyi/termcolor)
- [mini-yaml](https://github.com/jimmiebergmann/mini-yaml)
- [Detours](https://github.com/microsoft/Detours)

To be able to compile this project, download [this](https://github.com/jimmiebergmann/mini-yaml/tree/master/yaml) folder and move it to `./ExampleMod`

## Special Thanks

- **Kono** for making the logo
- **[Player]** for writing the original BDS DLL modding guide and inspiring me to write a new, updated one in english
- **! Pixel** for teaching me how BDS modding works
