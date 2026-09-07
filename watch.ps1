<#
    watch.ps1 - Watcher estilo-nodemon para el sandbox Wokwi.
    Vigila src\ (los .ino/.cpp/.h y cualquier otro archivo) y diagram.json.
    Con debounce de ~1.5s: tras el ultimo cambio, espera 1.5s y corre `pio run`.
    Ctrl+C para detener.
#>

$Root = $PSScriptRoot
$PioDir = Join-Path $env:USERPROFILE '.platformio\penv\Scripts'
$PioExe = Join-Path $PioDir 'pio.exe'

# Mismo mecanismo de PATH que setup.ps1.
if (-not (Get-Command pio -ErrorAction SilentlyContinue)) {
    if (Test-Path -LiteralPath $PioExe) {
        $env:PATH = "$PioDir;$env:PATH"
    }
}

if (-not (Get-Command pio -ErrorAction SilentlyContinue)) {
    Write-Host "ERROR: No se encontro 'pio'. Ejecuta setup.ps1 primero." -ForegroundColor Red
    exit 1
}

$DebounceMs = 1500

# Estado compartido mutable: el event handler y el loop principal leen/escriben aqui.
$state = [hashtable]@{
    Timer   = [System.Diagnostics.Stopwatch]::new()
    Pending = $false
}

function Invoke-Build {
    $stamp = Get-Date -Format 'yyyy-MM-dd HH:mm:ss'
    Write-Host ""
    Write-Host "[$stamp] Cambio detectado, compilando..." -ForegroundColor Cyan
    Push-Location $Root
    try {
        pio run
        $code = $LASTEXITCODE
    }
    finally {
        Pop-Location
    }
    $stamp2 = Get-Date -Format 'yyyy-MM-dd HH:mm:ss'
    if ($code -eq 0) {
        Write-Host "[$stamp2] [SUCCESS]" -ForegroundColor Green
    }
    else {
        Write-Host "[$stamp2] [ERROR] (exit $code)" -ForegroundColor Red
    }
}

# Watcher sobre src (recursivo) y sobre diagram.json en la raiz.
$SrcWatcher = [System.IO.FileSystemWatcher]::new()
$SrcWatcher.Path = Join-Path $Root 'src'
$SrcWatcher.IncludeSubdirectories = $true
$SrcWatcher.EnableRaisingEvents = $true

$DiagramWatcher = [System.IO.FileSystemWatcher]::new()
$DiagramWatcher.Path = $Root
$DiagramWatcher.Filter = 'diagram.json'
$DiagramWatcher.EnableRaisingEvents = $true

# El handler recibe el estado via MessageData (evita problemas de scope).
$OnChanged = {
    $s = $Event.MessageData
    $s.Timer.Restart()
    $s.Pending = $true
}

Register-ObjectEvent -InputObject $SrcWatcher -EventName Changed  -Action $OnChanged -MessageData $state | Out-Null
Register-ObjectEvent -InputObject $SrcWatcher -EventName Created  -Action $OnChanged -MessageData $state | Out-Null
Register-ObjectEvent -InputObject $SrcWatcher -EventName Deleted  -Action $OnChanged -MessageData $state | Out-Null
Register-ObjectEvent -InputObject $SrcWatcher -EventName Renamed  -Action $OnChanged -MessageData $state | Out-Null
Register-ObjectEvent -InputObject $DiagramWatcher -EventName Changed -Action $OnChanged -MessageData $state | Out-Null

Write-Host "Observando cambios en $($SrcWatcher.Path) y diagram.json" -ForegroundColor Cyan
Write-Host "Ctrl+C para detener." -ForegroundColor Cyan

try {
    while ($true) {
        if ($state.Pending -and $state.Timer.ElapsedMilliseconds -gt $DebounceMs) {
            $state.Pending = $false
            Invoke-Build
        }
        Start-Sleep -Milliseconds 200
    }
}
finally {
    Write-Host "`nDeteniendo watcher..." -ForegroundColor Yellow
    $SrcWatcher.EnableRaisingEvents = $false
    $DiagramWatcher.EnableRaisingEvents = $false
    $SrcWatcher.Dispose()
    $DiagramWatcher.Dispose()
    Get-EventSubscriber | Where-Object {
        $_.SourceObject -eq $SrcWatcher -or $_.SourceObject -eq $DiagramWatcher
    } | Unregister-Event | Out-Null
    Write-Host "Detenido." -ForegroundColor Yellow
}
