<#
    setup.ps1 - Deja el sandbox PlatformIO + Wokwi listo.
    Idempotente: se puede ejecutar tantas veces como se quiera.
    Uso:
        powershell -ExecutionPolicy Bypass -File setup.ps1
    o simplemente:
        .\setup.ps1
#>

$ErrorActionPreference = 'Stop'

$Root = $PSScriptRoot

# Buscar pio en multiples ubicaciones posibles.
$PioCandidates = @(
    (Join-Path $env:USERPROFILE '.platformio\penv\Scripts')
)

# Agregar la ruta Scripts de la instalacion de Python via pip global.
$PyScripts = $null
try {
    $PyScripts = & python -c "import sys,os; print(os.path.join(os.path.dirname(sys.executable),'Scripts'))" 2>$null
} catch {}
if ($PyScripts -and (Test-Path -LiteralPath (Join-Path $PyScripts 'pio.exe'))) {
    $PioCandidates += $PyScripts
}

$PioDir = $null
foreach ($Candidate in $PioCandidates) {
    $Exe = Join-Path $Candidate 'pio.exe'
    if (Test-Path -LiteralPath $Exe) {
        $PioDir = $Candidate
        break
    }
}

# 1) Hacer que 'pio' resuelva en esta sesion si no esta en el PATH actual.
if (-not (Get-Command pio -ErrorAction SilentlyContinue)) {
    if ($PioDir) {
        Write-Host "Agregando PlatformIO al PATH de la sesion: $PioDir" -ForegroundColor Cyan
        $env:PATH = "$PioDir;$env:PATH"
    }
}

# 2) Verificar que pio exista; si no, salir con codigo != 0.
if (-not (Get-Command pio -ErrorAction SilentlyContinue)) {
    Write-Host "ERROR: No se encontro 'pio'. Instalalo con: pip install platformio" -ForegroundColor Red
    exit 1
}

# 3) Verificar/crear los archivos base con template minimo si faltan.
function Ensure-File($Name, $Template) {
    $Path = Join-Path $Root $Name
    if (Test-Path -LiteralPath $Path) {
        Write-Host "[ok] $Name ya existe." -ForegroundColor Green
    }
    else {
        Set-Content -LiteralPath $Path -Value $Template -Encoding UTF8
        Write-Host "[creado] $Name" -ForegroundColor Yellow
    }
}

$MainIno = @'
void setup() {
  pinMode(LED_BUILTIN, OUTPUT);
}

void loop() {
  digitalWrite(LED_BUILTIN, HIGH);
  delay(1000);
  digitalWrite(LED_BUILTIN, LOW);
  delay(1000);
}
'@

$WokwiToml = @'
[wokwi]
version=1
firmware='.pio/build/uno/firmware.hex'
elf='.pio/build/uno/firmware.elf'
'@

$IniTemplate = @'
[env:uno]
platform = atmelavr
board = uno
framework = arduino
'@

$DiagramJson = @'
{
  "version": 1,
  "editor": "wokwi",
  "parts": [
    {
      "type": "wokwi-arduino-uno",
      "id": "uno",
      "top": 0,
      "left": 0,
      "attrs": {}
    }
  ],
  "connections": [],
  "dependencies": {}
}
'@

Ensure-File 'src/main.ino' $MainIno
Ensure-File 'wokwi.toml' $WokwiToml
Ensure-File 'platformio.ini' $IniTemplate
Ensure-File 'diagram.json' $DiagramJson

# 4) Compilar el proyecto (desde la raiz del sandbox, sin importar el CWD actual).
Write-Host "`nEjecutando pio run..." -ForegroundColor Cyan
Push-Location $Root
try {
    pio run
    $code = $LASTEXITCODE
}
finally {
    Pop-Location
}
if ($code -ne 0) {
    Write-Host "ERROR: pio run fallo." -ForegroundColor Red
    exit $code
}

# 5) Mensaje final.
Write-Host ""
Write-Host "Sandbox listo. Abri VS Code, Wokwi: Start Simulator." -ForegroundColor Green
exit 0
