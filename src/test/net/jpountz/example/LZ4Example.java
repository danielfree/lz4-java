package net.jpountz.example;

/*
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

import java.io.File;
import java.io.FileInputStream;
import java.io.FileNotFoundException;
import java.io.FileOutputStream;
import java.io.UnsupportedEncodingException;

import net.jpountz.lz4.LZ4CompatibleInputStream;
import net.jpountz.lz4.LZ4CompatibleOutputStream;
import net.jpountz.lz4.LZ4Compressor;
import net.jpountz.lz4.LZ4Factory;
import net.jpountz.lz4.LZ4FastDecompressor;
import net.jpountz.lz4.LZ4SafeDecompressor;

public class LZ4Example {

  public static void main(String[] args) throws Exception {
    example();
  }

  private static void example() throws UnsupportedEncodingException {
    LZ4Factory factory = LZ4Factory.fastestInstance();

    byte[] data = "12345345234572".getBytes("UTF-8");
    final int decompressedLength = data.length;

    // compress data
    LZ4Compressor compressor = factory.fastCompressor();
    int maxCompressedLength = compressor.maxCompressedLength(decompressedLength);
    byte[] compressed = new byte[maxCompressedLength];
    int compressedLength = compressor.compress(data, 0, decompressedLength, compressed, 0, maxCompressedLength);

    // decompress data
    // - method 1: when the decompressed length is known
    LZ4FastDecompressor decompressor = factory.fastDecompressor();
    byte[] restored = new byte[decompressedLength];
    int compressedLength2 = decompressor.decompress(compressed, 0, restored, 0, decompressedLength);
    // compressedLength == compressedLength2

    // - method 2: when the compressed length is known (a little slower)
    // the destination buffer needs to be over-sized
    LZ4SafeDecompressor decompressor2 = factory.safeDecompressor();
    int decompressedLength2 = decompressor2.decompress(compressed, 0, compressedLength, restored, 0);
    // decompressedLength == decompressedLength2

    File inputFile = new File("/Users/Daniel/Desktop/js.txt");
    File outputFile = new File("/Users/Daniel/Desktop/js-lz4.txt");
    try {
      FileInputStream inputStream = new FileInputStream(inputFile);
      FileOutputStream outputStream = new FileOutputStream(outputFile);
      LZ4CompatibleOutputStream lz4CompatibleOutputStream = new LZ4CompatibleOutputStream(outputStream);
      byte[] buffer = new byte[64*1024];
      while (inputStream.available() > 0) {
        int read = inputStream.read(buffer);
        lz4CompatibleOutputStream.write(buffer,0, read);
      }
      inputStream.close();
      lz4CompatibleOutputStream.close();


      System.out.println("compress complete, now start to decompress...");


      inputStream = new FileInputStream(outputFile);
      File decompressFile = new File("/Users/Daniel/Desktop/js-decompress.txt");
      outputStream = new FileOutputStream(decompressFile);
      LZ4CompatibleInputStream lz4CompatibleInputStream = new LZ4CompatibleInputStream(inputStream);
      buffer = new byte[5*1024];
      int read = lz4CompatibleInputStream.read(buffer, 0, buffer.length);
      outputStream.write(buffer,0, read);
      buffer = new byte[64*1024];
      read = lz4CompatibleInputStream.read(buffer, 0, buffer.length);
      while ( read != -1) {
        outputStream.write(buffer, 0, read);
        read = lz4CompatibleInputStream.read(buffer, 0, buffer.length);
      }
      lz4CompatibleInputStream.close();
      outputStream.close();
      System.out.println("decompress complete");
    } catch (Exception e) {
      e.printStackTrace();
    }

  }

}
