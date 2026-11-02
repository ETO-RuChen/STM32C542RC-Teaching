param(
    [string]$Root = (Get-Location).Path
)

$ErrorActionPreference = 'Stop'
$required = @('README.md', 'AGENTS.md', '.codex/project.yaml', 'docs/10-context-handoff.md', 'labs/index.md')
$missing = @($required | Where-Object { -not (Test-Path (Join-Path $Root $_)) })
if ($missing.Count -gt 0) {
    Write-Error ("Missing required files: " + ($missing -join ', '))
}

$labFiles = @(Get-ChildItem (Join-Path $Root 'labs') -Recurse -Filter lab.yaml -File)
if ($labFiles.Count -eq 0) {
    Write-Error 'No lab.yaml files found.'
}

Write-Output ("Repository validation passed: {0} lab contract(s)." -f $labFiles.Count)
