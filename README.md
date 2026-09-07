# Wokwi Sandbox

Sandbox de prueba local para **Arduino Uno + Wokwi**, basado en PlatformIO.

Un proyecto simple y aislado para experimentar con código Arduino y simularlo localmente con
la extensión [Wokwi for VS Code](https://marketplace.visualstudio.com/items?itemName=wokwi.wokwi-vscode)
sin necesidad de hardware.

## Estructura

```
src/main.ino      Codigo Arduino (blink por defecto)
platformio.ini    Config de PlatformIO (env:uno, atmelavr, arduino)
wokwi.toml        Apunta a .pio/build/uno/firmware.hex y .elf
diagram.json      Diagrama del circuito en Wokwi
setup.ps1         Deja todo listo (idempotente)
watch.ps1         Watcher opcional que recompila al cambiar codigo
```

## Uso

### 1. Setup inicial

Desde la raiz del sandbox:

```
powershell -ExecutionPolicy Bypass -File setup.ps1
```

(el `-ExecutionPolicy Bypass` solo hace falta si PowerShell bloquea scripts).
Este script:

- Agrega PlatformIO al PATH de la sesion si `pio` no se resuelve.
- Verifica/crea `src/main.ino`, `platformio.ini`, `wokwi.toml` y `diagram.json` si faltan.
- Corre `pio run`.

Debes ver: **"Sandbox listo. Abri VS Code, Wokwi: Start Simulator."**

### 2. Simular

1. Abri esta carpeta en VS Code con la extension Wokwi instalada.
2. Presioná **F1** y elegi **"Wokwi: Start Simulator"**.

### 3. Recompilar ante cambios

En vez de correr `pio run` a mano cada vez, podes dejar el watcher corriendo:

```
.\watch.ps1
```

Vigila `src\` y `diagram.json`; tras ~1.5s sin cambios, recompila y loguea `[SUCCESS]` o `[ERROR]`.
Ctrl+C para detener.

## Flujo basico

1. Editá `src/main.ino`.
2. Compilá: `pio run` (o el watcher).
3. Wokwi: Start Simulator para verlo.
