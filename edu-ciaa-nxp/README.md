# ciaa-example-makefile
Programa de prueba para la placa EDU-CIAA-NXP, tomado de los ejemplos provisto en el IDE [CIAA Suite](https://github.com/ciaa/Software-IDE/releases). A continuación se describe como importarlo en Eclipse y ejecutarlo en la placa.

---

## Clonar el proyecto
Clonar el proyecto usando Git:
* Desde un cliente, usar la opción **[Clone]**.
* Desde la linea de comandos utilizar el comando `git clone`.

*Tip*: pueden probar también realizar un *fork* del proyecto.

---

## Importar el proyecto en Eclipse
Una vez clonado localmente, agregar el proyecto en Eclipse:
* Seleccionar **[File > New > Makefile project with existing code]**. 
* En la nueva ventana:
    * En **[Existing Code Location]** indicar el *path* al proyecto (usar el botón **[Browse...]**).
    * En **[Toolchain for Indexer]** seleccionar la opción *ARM Cross GCC* (¡importante!).

El proyecto debe aparecer ahora en la vista *Project Explorer*: 
* Hacer clic derecho sobre el mismo, y seleccionar **[Properties]** en el menú contextual.
* En la nueva ventana, seleccionar **[C/C++ Build > Settings]**. En la sección derecha de la ventana, en la pestaña **[Toolchains]**, el campo *Name* debe indicar *GNU MCU Eclipse ARM Embedded GCC (arm-none-eabi-gcc)* o bien *GNU Tools for ARM Embedded Processors*.
* Hacer clic en **[Ok]**.

---

## Compilar
Realizar una copia del archivo `Makefile.config` con el nombre `Makefile.mine`. En este nuevo archivo, se indica cual de las aplicaciones incluidas se debe compilar, modificando la variable `PROJECT_NAME`. Por defecto se compila la aplicación `example`.

Finalmente, para compilar el proyecto se puede:
* Hacer clic derecho sobre el proyecto en la vista *Project Explorer* y seleccionar **[Build]** en el menú contextual.
* Seleccionar en la barra de menúes de Eclipse **[Project > Build Project]**.
* Hacer clic en el ícono *Build* (un martillo).

Si el proyecto compilo correctamente, en la vista **[Console]** debe indicarse que se generó correctamente el archivo `apps/example/out/example.elf`.

---

## Configurar reglas udev

Crear el archivo `/etc/udev/rules.d/edu-ciaa-nxp.rules` con el siguiente contenido:

```
SUBSYSTEM=="usb", ATTR{idVendor}=="0403", ATTR{idProduct}=="6010", MODE:="666"
```

Luego, para actualizar las reglas del sistema sin necesidad de reiniciar, ejecutar:
```
$ sudo udevadm control --reload
$ sudo udevadm trigger
```

---

## Configurar entorno OpenOCD
Primero, verificar que `openocd` este correctamente configurado en Eclipse:
* Seleccionar **[Windows > Preferences]** en el menú de Eclipse.
* En la nueva ventana, seleccionar **[MCU > Global OpenOCD Path]** en la lista izquierda.
* En el campo *Executable* debe indicar `openocd.exe` (sin la extensión en Linux o MacOS), y el campo *Folder* debe contener el *path* al ejecutable.
* Hacer clic en **[Apply and Close]**.

A continuación se creará y ejecutará una configuración de *debug*:
* Seleccionar **[Run > Debug Configurations...]** en el menú de Eclipse.
* En la nueva ventana, hacer doble clic sobre **[GDB OpenOCD debugging]** en el menú izquierdo. Esto crea una nueva configuración basada en este perfil, con el nombre del proyecto activo.
* Seleccionar el nuevo perfil creado (*ciaa-example-makefile Default*).
* En el panel derecho:
    * En la pestaña **[Main]**, el campo *Project* debe indicar el nombre del proyecto ('ciaa-example-makefile'). El campo *C/C++ Application* debe indicar el nombre del archivo ELF, en este caso `app/out/app.elf`. Si no estuviera presente escribir el nombre del mismo, o bien hacer clic en el botón **[Search project]** o **[Browse]** para buscarlo.
    * En la pestaña **[Debugger]**, el campo *Executable* debe contener el valor `${openocd_path}/${openocd_executable}`. Estas dos variables son reemplazadas por los valores especificados el menú **[MCU > Global OpenOCD Path]**. Además, en el campo *Config options*, se debe ingresar `-f scripts/openocd/lpc4337.cfg`.
    * En la pestaña **[Common]**, seleccionar la opción **[Shared file:]**, indicando en el campo el nombre del proyecto. De esta manera la configuración para debugging es guardada en un archivo `*.launch` dentro del proyecto.
    * Hacer clic en el botón **[Apply]**. No cerrar la ventana aún.
    
Para ejecutar el programa por primera vez, conectar la placa si no lo estuviera aún y, hacer clic en el botón **[Debug]**, que esta en la parte inferior de la ventana. Es posible que Eclipse pregunte si se desea cambiar a la perspectiva *Debug*, responder que sí.

---
    
## Próximas ejecuciones
Para cargar el programa en la placa y ejecutarlo, la opción más sencilla es hacer clic en el ícono **[Debug]**, en la barra de herramientas de Eclipse, para ejecutar nuevamente el último perfil utilizado. Si se desea cambiar el perfil, hacer clic en la flecha que se encuentra junto al ícono **[Debug]** y de la lista desplegable que se presenta, seleccionar el perfil deseado, o hacer clic en la opción **[Debug Configurations...]**, para abrir la ventana de configuración vista en el punto anterior.
