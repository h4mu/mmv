# PowerShell script to replicate mmv.cpp functionality

# Add the C# code for P/Invoke
$cSharpCode = @"
using System;
using System.Runtime.InteropServices;

public static class PowerHelper
{
    [Flags]
    public enum EXECUTION_STATE : uint
    {
        ES_AWAYMODE_REQUIRED = 0x00000040,
        ES_CONTINUOUS = 0x80000000,
        ES_DISPLAY_REQUIRED = 0x00000002,
        ES_SYSTEM_REQUIRED = 0x00000001
    }

    [DllImport("kernel32.dll", CharSet = CharSet.Auto, SetLastError = true)]
    public static extern EXECUTION_STATE SetThreadExecutionState(EXECUTION_STATE esFlags);
}
"@

Add-Type -TypeDefinition $cSharpCode -Language CSharp

# Load assemblies and set up the form
Add-Type -AssemblyName System.Windows.Forms
Add-Type -AssemblyName System.Drawing

$form = New-Object System.Windows.Forms.Form
$form.Text = "MMV"
$form.WindowState = [System.Windows.Forms.FormWindowState]::Maximized
$form.FormBorderStyle = [System.Windows.Forms.FormBorderStyle]::FixedSingle

$label = New-Object System.Windows.Forms.Label
$label.Text = "Mouse Mover"
$label.Font = New-Object System.Drawing.Font("Arial", 24, [System.Drawing.FontStyle]::Bold)
$label.AutoSize = $true
$form.Controls.Add($label)

# Center the label
$form.Add_Load({
    $label.Left = ($form.ClientSize.Width - $label.Width) / 2
    $label.Top = ($form.ClientSize.Height - $label.Height) / 2
})

# Set up timer
$timer = New-Object System.Windows.Forms.Timer
$timer.Interval = 10000 # 10 seconds
$timer.Add_Tick({
    $random = New-Object Random
    $x = $random.Next($form.ClientSize.Width)
    $y = $random.Next($form.ClientSize.Height)
    $screenCoords = $form.PointToScreen((New-Object System.Drawing.Point($x, $y)))
    [System.Windows.Forms.Cursor]::Position = $screenCoords
})

# Prevent sleep
[PowerHelper]::SetThreadExecutionState([PowerHelper+EXECUTION_STATE]::ES_DISPLAY_REQUIRED -bor [PowerHelper+EXECUTION_STATE]::ES_CONTINUOUS)

$timer.Start()

# Handle form closing
$form.Add_FormClosing({
    $timer.Stop()
    # Restore system sleep settings
    [PowerHelper]::SetThreadExecutionState([PowerHelper+EXECUTION_STATE]::ES_CONTINUOUS) | Out-Null
    $timer.Dispose()
    $label.Dispose()
    $form.Dispose()
})

# Show the form
$form.ShowDialog()
