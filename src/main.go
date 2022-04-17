package main

import (
	. "fmt"
	. "os"
	. "strconv"
	"syscall"
	. "time"
)

const (
    pause = 19
    resume = 18
    stop = 9
)



func is_arg() bool {
    switch {
    case Args[1] == "start":
        return true
    case Args[1] == "pause":
        return true
    case Args[1] == "clock":
        return true
    case Args[1] == "stop":
        return true
    case Args[1] == "resume":
        return true
    }
    return false
}

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

func main() {
    if len(Args) < 1 || !is_arg() {
        Println(help())
    }
    if Args[1] == "start" {
        start_opt()
    }
    if Args[1] == "stop" {
        pid, _ := Atoi(Args[2])
        signal_opt(pid, stop)
    }
    if Args[1] == "pause" {
        pid, _ := Atoi(Args[2])
        signal_opt(pid, pause)
    }
    if Args[1] == "resume" {
        pid, _ := Atoi(Args[2])
        signal_opt(pid, resume)
    }
    if Args[1] == "clock" {
        clock_opt()
    }
}

func start_opt() {
    count := 25
    for count > 0 {
        Sleep(Second)
        count--
    }
}

func signal_opt(pid int, sig int) {
    var err error
    err = syscall.Kill(pid, sig) 

    if err == nil {
        Println("failure")
    }
}

func clock_opt() {

}
