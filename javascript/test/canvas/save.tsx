import { CanvasRenderingContext2D } from "../../meui/canvas"

export default function Test(ctx: CanvasRenderingContext2D) {
    // https://developer.mozilla.org/zh-CN/docs/Web/API/CanvasRenderingContext2D/save

    ctx.save(); // 保存默认的状态

    ctx.fillStyle = "green";
    ctx.fillRect(10, 10, 100, 100);
    
    ctx.restore(); // 还原到上次保存的默认状态
    ctx.fillRect(150, 75, 100, 100);
}
