/* global globalThis */
import * as os from 'os'
globalThis.process = { 
    env: { NODE_ENV: 'development' },
    platform: os.platform,
    cwd: () =>{
        const [path, err]  = os.getcwd()
        if(err !== 0 )
            throw new Error(" os.cwd() errno")
        return path
    }
 }
if (!globalThis.setTimeout) globalThis.setTimeout = os.setTimeout
globalThis.console.warn = console.log
globalThis.console.error = () => {}