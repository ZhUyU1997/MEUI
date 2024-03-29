/*
 * SPDX-FileCopyrightText: 2022 Yu Zhu <891085309@qq.com>
 *
 * SPDX-License-Identifier: MIT
 */

declare module "os" {
    export function open(filename: string, flags: number, mode: number): number
    export const O_RDONLY: number
    export const O_WRONLY: number
    export const O_RDWR: number
    export const O_APPEND: number
    export const O_CREAT: number
    export const O_EXCL: number
    export const O_TRUNC: number
    export const O_BINARY: number
    export const O_TEXT: number
    export function close(fd: number): number
    export function seek(
        fd: number,
        pos: number | BigInt,
        whence: number
    ): number | BigInt
    export function read(fd: number, pos: number, len: number): number
    export function write(fd: number, pos: number, len: number): number
    export function isatty(fd: number): boolean
    export function ttyGetWinSize(fd: number): [number, number]
    export function ttySetRaw(fd: number): void
    export function remove(fd: number): number
    export function rename(oldpath: string, newpath: string): number
    export function setReadHandler(fd: number, func: Function | null): void
    export function setWriteHandler(fd: number, func: Function | null): void
    export function setEventHandler(func: Function | null): void
    export function signal(sig_num: number, func: Function | null): void
    export const SIGINT: number
    export const SIGABRT: number
    export const SIGFPE: number
    export const SIGILL: number
    export const SIGSEGV: number
    export const SIGTERM: number
    export const SIGQUIT: number
    export const SIGPIPE: number
    export const SIGALRM: number
    export const SIGUSR1: number
    export const SIGUSR2: number
    export const SIGCHLD: number
    export const SIGCONT: number
    export const SIGSTOP: number
    export const SIGTSTP: number
    export const SIGTTIN: number
    export const SIGTTOU: number
    export function setTimeout(func: Function, delay: number): any
    export function clearTimeout(timer: any): void
    export const platform: string
    export function getcwd(): [string, number]
    export function chdir(target: string): number
    export function mkdir(path: string, mode?: number): number
    export function readdir(path: string): [string[], number]
    export const S_IFMT: number
    export const S_IFIFO: number
    export const S_IFCHR: number
    export const S_IFDIR: number
    export const S_IFBLK: number
    export const S_IFREG: number
    export const S_IFSOCK: number
    export const S_IFLNK: number
    export const S_ISGID: number
    export const S_ISUID: number
    export type stat_return_type = {
        dev: number
        ino: number
        mode: number
        nlink: number
        uid: number
        gid: number
        rdev: number
        size: number
        block: number
        atime: number
        mtime: number
        ctime: number
    }
    export function stat(path: string): [stat_return_type, number]
    export function utimes(path: string, atime: number, mtime: number): number
    export function sleep(delay: number): number
    export function realpath(path: number): [string, number]
    export function lstat(path: string): [stat_return_type, number]
    export function symlink(target: string, linkpath: string): number
    export function readlink(path: string): [string, number]
    export function exec(
        args: string[],
        options?: {
            block?: boolean
            usePath?: boolean
            file?: string
            cwd?: string
            stdin?: number
            stdout?: number
            stderr?: number
            env?: Record<string, string>
            uid?: number
            gid?: number
        }
    ): number
    export function waitpid(pid: number, options?: number): [number, number]
    export const WNOHANG: number
    export function pipe(): [number, number]
    export function kill(pid: number, sig: number): number
    export function dup(fd: number): number
    export function dup2(fd: number, fd2: number): number
}
