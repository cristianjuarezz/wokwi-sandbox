<#
    run.ps1 - Compila y corre la simulacion Wokwi headless de una sola pasada.
    Uso:
        .\run.ps1

    Hace:
        1) Verifica pio y corre `pio run` (mismo mecanismo que setup.ps1).
        2) Si pio run falla, deja el error de compilacion completo en pantalla
           y termina con exit code 1 (asi el dueno ve si es problema de SU codigo).
        3) Ejecuta wokwi-cli headless sobre la raiz del sandbox, usando el
           diagram.json real y respetando wokwi.toml (NO genera diagramas).
        4) El serial output de la simulacion se imprime en la terminal.
           La simulacion corta sola por timeout (~18s, exit code 42 = timeout
           normal del CLI) o se puede cortar con Ctrl+C.
#>

$ErrorActionPreference = 'Stop'

$Root = $PSScriptRoot

# Buscar pio en multiples ubicaciones posibles (igual que setup.ps1).
$PioExe = Join-Path $env:USERPROFILE '.platformio\penv\Scripts\pio.exe'

# 1) Verificar pio.
if (-not (Get-Command pio -ErrorAction SilentlyContinue)) {
    if (Test-Path -LiteralPath $PioExe) {
        $PioDir = Split-Path -Parent $PioExe
        Write-Host "Agregando PlatformIO al PATH de la sesion: $PioDir" -ForegroundColor Cyan
        $env:PATH = "$PioDir;$env:PATH"
    }
}
if (-not (Get-Command pio -ErrorAction SilentlyContinue)) {
    Write-Host "ERROR: No se encontro 'pio'. Ejecuta setup.ps1 primero." -ForegroundColor Red
    exit 1
}

# 2) Compilar el proyecto (desde la raiz del sandbox).
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
    # Si pio fallo, el error de compilacion completo ya quedo impreso arriba.
    Write-Host "`nERROR: pio run fallo (exit $code). El error de compilacion completo esta arriba." -ForegroundColor Red
    exit 1
}

# 3) Verificar binario de wokwi-cli.
$WokwiCli = Join-Path $Root '.tools\wokwi-cli\wokwi-cli.exe'
if (-not (Test-Path -LiteralPath $WokwiCli)) {
    Write-Host "ERROR: No se encontro wokwi-cli en $WokwiCli" -ForegroundColor Red
    exit 1
}

# 4) Correr la simulacion headless.
#    - Se le pasa la raiz del proyecto como path: wokwi-cli lee wokwi.toml
#      (firmware/elf) y usa el diagram.json real. No se genera ningun diagrama.
#    - --timeout 18000: la simulacion corta sola a los 18s (exit code 42
#      por default del CLI, esperado). Tambien se puede cortar con Ctrl+C.
#    - Sin --quiet: el serial output de la simulacion se ve en la terminal.
Write-Host "`nIniciando simulacion Wokwi (timeout 18s, Ctrl+C para cortar)..." -ForegroundColor Cyan
Write-Host "------------------------------------------------------------" -ForegroundColor DarkGray

Push-Location $Root
try {
    & $WokwiCli $Root --timeout 18000
    $wcode = $LASTEXITCODE
}
finally {
    Pop-Location
}

# 5) Interpretar el resultado.
if ($wcode -eq 42) {
    Write-Host ""
    Write-Host "Simulacion terminada por timeout ($wcode, esperado)." -ForegroundColor Yellow
    exit 0
}
elseif ($wcode -eq 0) {
    Write-Host ""
    Write-Host "Simulacion terminada (exit 0)." -ForegroundColor Green
    exit 0
}
else {
    Write-Host ""
    Write-Host "ERROR: wokwi-cli termino con exit code $wcode." -ForegroundColor Red
    exit $wcode
}