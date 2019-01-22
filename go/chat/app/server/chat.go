package chat

import (
	"flag"
	"log"
)

func init()  {
	log.SetFlags(log.Ldate | log.Lshortfile)
}

func ServerStart()  {
	flag.Parse()


}