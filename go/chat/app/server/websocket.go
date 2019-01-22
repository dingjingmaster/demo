package chat

import "lib/gorilla/websocket"

const (
	SystemMessage = iota				// 系统消息
	BroadCastMessage					// 广播消息
	HeartBeatMessage					// 心跳消息
	ConnectedMessage					// 上线通知
	DisConnectedMessage					// 下线通知
)

var aliveList *AliveList

/* 当前在线列表 */
type AliveList struct {

}

/* Client socket 客户端 */
type Client struct {
	ID 				string
	conn 			*websocket.Conn
}