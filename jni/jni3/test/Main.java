import java.io.*;

public class Main
{
    public static void main (String[] args) {
        if (args.length != 2) {
            System.out.println ("输入要加密的文件，和加密后保存的文件路径");
            System.exit(1);
        }

        try {
            File f1 = new File(args[0]);
            if (!f1.exists()) {
                System.out.println("文件不存在");
                System.exit(1);
            }

            File f2 = new File(args[1]);
            if (!f2.exists()) {
                f2.createNewFile();
            }

            FileInputStream f1I = new FileInputStream(f1);
            FileOutputStream f2O = new FileOutputStream(f2);
            long fileOffset = 0;
            int[] fileHeadSize = new int[1];
            byte[] key = new byte[1024];
            int[] keyLen = new int[1];
            byte[] head = new byte[1024];
            byte[] data = new byte[1024];
            byte[] headExtendData = new byte[1024];

            fileHeadSize[0] = 1580;
            boolean isFirst = true;
            while (-1 != f1I.read(data)) {
                int ret = 0;
                new Main().encryptBlock(
                        "fileID".getBytes(), 
                        (byte)0x10/* DRM */,
                        f1.length()/*文件大小*/,
                        data/*加密块*/,
                        data.length/*加密块大小*/,
                        fileOffset/*文件偏移*/,
                        "AKAKAKAKAKAKAKAK".getBytes()/*加密密钥*/,
                        16/*加密密钥长度*/,
                        null/*加密头*/,
                        head/*加密头*/,
                        fileHeadSize/*加密头大小*/,
                        key/*加密时候随机密钥数据*/,
                        keyLen/*随机密钥大小*/,
                        -1,
                        headExtendData,
                        30,
                        args[1].getBytes(),
                        "tid".getBytes(),
                        "uid".getBytes(),
                        "user name".getBytes(),
                        0,
                        (long)0);
                if (0 != ret) {
                    System.out.printf("出错: %d\n", ret);
                    fileOffset += data.length;
                }
                else {
                    if(isFirst) {
                       f2O.write(head);
                    }
                    f2O.write(data, 0, data.length);
                }
                isFirst = false;
            }
        } catch (Exception e) {
            System.out.println("失败!");
            System.out.println(e);
        }
    }

    private native int encryptBlock (
                    final byte[]    fileID, 
                    byte            fileType,
                    long            fileSize,
                    byte[]          block,
                    int             blockLen,
                    long            fileOffset,
                    byte[]          cryptKey,
                    int             cryptKeyLen,
                    byte[]          inFileHead,
                    byte[]          outFileHead,
                    int[]           outFileHeadSize,
                    byte[]          dataKey,
                    int[]           dataKeyLen,
                    int             headExtendVer,
                    byte[]          headExtendData,
                    int             headExtendDataLen,
                    byte[]          szFileName,
                    byte[]          szTid,
                    byte[]          szUserId,
                    byte[]          szUserName,
                    int             validDate,
                    long            fileMakeTime
                    );
    static {
        System.loadLibrary("encrypt-jar");
    }
}
