PLC Industrial simulado (PLC-SIM) que controla una instalación fotovoltaica
=============

Este PLC Industrial simulado (PLC-SIM) es uno de los componentes del escenario que se ha puesto en marcha para la realización de una prueba de concepto (PoC) que tiene como objetivo analizar las posibilidades de explotación de vulnerabilidades en entornos del Internet Industrial de las Cosas (IIoT). Este escenario está compuesto además por otros dos elementos: un componente IT, un [servidor web](https://github.com/fjdcordon/tfg-servidorweb) para la monitorización y control de la infraestructura a distancia; y un componente IoT, un [Gateway IoT](https://github.com/fjdcordon/tfg-gateway) que hace de interfaz IT/OT.

Para más información sobre el desarrollo de este escenario y la verificación de esta prueba de concepto se puede consultar mi Trabajo de Fin de Grado: «Análisis y explotación de vulnerabilidades en un sistema de monitorización y control remoto de energía solar».

Autor: Francisco Javier Domínguez Cordón

Instalación
-------------

1. Dado que la infraestructura está virtualizada, hay que descargar un software de virtualización. En este caso, se ha optado por utilizar [Oracle VM VirtualBox](https://www.virtualbox.org/), un software de código abierto, gratuito y multiplataforma (Windows, GNU/Linux, Mac OS). Para instalarlo simplemente hay que seguir los pasos que se detallan en su página web.

2. Descargar la imagen de máquina virtual de ArchLinux preparada para VirtualBox:  [Descargar Máquina](https://gitlab.archlinux.org/archlinux/arch-boxes/-/jobs/63545/artifacts/file/output/Arch-Linux-x86_64-virtualbox-20220623.63545.box)

3. Una vez descargado el archivo, lo descomprimiremos y localizaremos el fichero *box.ovf*. Ahora, abrimos VirtualBox abrimos el menú de Archivo o *File* (dependiendo del idioma en que este configurado el equipo anfitrión) y haremos click sobre *Importar servicio virtualizado* o *Import Appliance*. Como fuente elegiremos *Sistema local* o *Local File System* y elegiremos el mencionado archivo *box.ovf*. Pulsaremos en continuar, pondremos el nombre que deseemos a la máquina y dejaremos el resto de opciones por defecto.

4. Ahora iniciaremos la máquina. Cuando lleguemos a la pantalla de *login* introduciremos como usuario y contraseña *vagrant*. A continuación, ejecutaremos el comando *sudo passwd* y pondremos la contraseña que deseemos para el usuario *root*. Después cerraremos sesión con el comando *logout* para, posteriormente, volver a iniciar sesión como root con la contraseña que acabamos de establecer.

5. Con la sesión de root iniciada ejecutaremos los siguientes comandos:

```bash
localectl set-keymap --no-convert es
pacman -Syyu git python-pip
git clone https://github.com/fjdcordon/tfg-plc
cd tfg-plc
sudo ./setup.sh
```
El primer comando sirve para cambiar el *layout* del teclado al Español. Se pueden consultar todos los tipos de *layout* con el comando  *localectl list-keymaps*.

4. Una vez finalizada la ejecución del script, la máquina se apagará. Antes de volver a encenderla, es necesario ir a la configuración de red de la máquina y cambiar el único adaptador que tiene de modo *NAT* a modo *Red Interna* o *Internal Network*. El nombre de la red interna es el que viene por defecto: *intnet*.

5. Ahora volvemos a iniciar la máquina y ya tendremos el componente totalmente funcional. 

Es MUY importante destacar que, para que los otros dos componentes no podrán funcionar si este no esta encendido.
