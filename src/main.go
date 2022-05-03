package main

import (
	. "fmt"
	. "os"
	"os/exec"
	"os/signal"
	. "strconv"
	"strings"
	"syscall"
	. "time"
)

const (
    pause = 19
    resume = 18
    stop = 9
    usr1 = 10
    usr2 = 11
)

func help() string {
    return `
    Examples:
        $> pomo [start|stop|pause|clock|resume]
    
    Command:
        [start] begin the timer
        [pause] pause the timer 
        [resume] resume the timer
        [clock] print remaining time
        [stop]  stop the timer
    `
}

func gest_arg() bool {
    var pid int

    if Args[1] != "start" {
        cmd := exec.Command("pgrep", "-o", "pomo")
        stdout, _ := cmd.Output()
        s := string(stdout)
        s = strings.TrimSuffix(s, "\n")
        p, err := ParseInt(s, 10, 64)
        if err != nil {
            Println(err)
        }
        pid = int(p)
    }
    switch {
    case Args[1] == "start":
        start_opt()
    case Args[1] == "pause":
        signal_opt(pid, pause)
    case Args[1] == "clock":
        signal_opt(pid, usr1)
    case Args[1] == "stop":
        signal_opt(pid, stop)
    case Args[1] == "resume":
        signal_opt(pid, resume)
    }
    return false
}

func start_opt() {
    c := make(chan Signal, 1)
    signal.Notify(c, syscall.SIGUSR1, syscall.SIGUSR2)
    sec := 59
    min := 24
    for min > 0 {
        Sleep(Second)
        sec--
        if sec == 0 {
            min--
            sec = 59
        }
        go func() {
            test := <-c
            if test != nil {
                format := Itoa(min) + ":" + Itoa(sec)
                Println("format : ", format)
                cmd := exec.Command("notify-send", 
                "-i", "/home/rr/Documents/Github/Pomo/res/pomodoro.jpg",
                "Pomodoro", format)
                err := cmd.Run()
                if err != nil {
                    panic(err)
                }
            }
        }()
    }
    Exit(0)
}

func signal_opt(pid int, sig syscall.Signal) {
    err := syscall.Kill(pid, sig) 
    if err != nil {
        Println("failure")
    }
}

func main() {
    if len(Args) <= 1 || !gest_arg() {
        Println(help())
        Exit(1)
    }
}
