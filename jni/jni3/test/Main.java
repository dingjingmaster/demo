import java.io.*;

public class Main
{
    public static void main (String[] args) {
        if (args.length != 2) {
            System.out.println ("输入要加密的文件");
            System.exit(1);
        }
        File f1 = new File(args[1]);
        if (!f1.exists()) {
            System.out.println("文件不存在");
            System.exit(1);
        }
    }

    private native int encryptBlock (String fileID, 
                    char fileType,
                    long fileSize,
                    char[] block,
                    int     blockLen,
                    long    fileOffset,
                    char[]  cryptKey,
                    short   cryptKeyLen,
                    char[]  inFileHead,
                    char[]  outFileHead,
                    short[] outFileHeadSize,
                    char[]  dataKey,
                    short[] dataKeyLen,
                    short[] headExtendVer,
                    short[] headExtendDataLen,
                    char[]  szFileName,
                    char[]  szTid,
                    char[]  szUserId,
                    char[]  szUserName
                    );

}
