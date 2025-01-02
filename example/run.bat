@echo off

if exist bin/debug/ExampleProject.exe (
    start bin/debug/ExampleProject.exe
) else (
    echo Could not find executable!
)