declare module 'std' {
    export class FILE {
        close(): number;
        puts(...args: string[]): void;
        printf(fmt: string, ...args: (string | number)[]): number;
        flush(): void;
        tell(): number;
        tello(): BigInt;
        seek(pos: number, whence: number): number;
        eof(): boolean;
        fileno(): number;
        error(): boolean;
        clearerr(): void;
        read(buf: ArrayBuffer, pos: number, len: number): number;
        write(buf: ArrayBuffer, pos: number, len: number): number;
        getline(): string;
        readAsString(maxSize?: number): string;
        getByte(): number;
        putByte(): number;
    }
    export function exit(): void;
    export function gc(): void;
    export function evalScript(str: string, option: {
        backtrace_barrier: boolean;
    }): any;
    export function loadScript(filename: string): any;
    export function getenv(): string | undefined;
    export function setenv(name: string, value: string): void;
    export function unsetenv(name: string): void;
    export function getenviron(): Record<string, string>;
    export function urlGet(url: string, option: {
        binary: boolean;
        full: boolean;
    }): void;
    export function loadFile(filename: string): string;
    export function strerror(): string;
    export function parseExtJSON(str: string): any;
    export function open(filename: string, mode: string, err?: {}): FILE;
    export function popen(filename: string, mode: string, err?: {}): FILE;
    export function fdopen(fd: number, mode: string, err?: {}): FILE;
    export function tmpfile(err?: {}): FILE;
    export function puts(...args: string[]): void;
    export function printf(fmt: string, ...args: (string | number)[]): number;
    export function sprintf(fmt: string, ...args: (string | number)[]): string;
    export const SEEK_SET: number;
    export const SEEK_CUR: number;
    export const SEEK_END: number;
    type errnoList = "EINVAL" | "EIO" | "EACCES" | "EEXIST" | "ENOSPC" | "ENOSYS" | "EBUSY" | "ENOENT" | "EPERM" | "EPIPE" | "EBADF";
    export const Error: Record<errnoList, number>;

    export const in: FILE;
    export const out: FILE;
    export const err: FILE;
}