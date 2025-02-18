#include "Crypto.h"
#include "GrayscaleImage.h"
#include <bitset>

// Extract the least significant bits (LSBs) from SecretImage, calculating x, y based on message length
std::vector<int> Crypto::extract_LSBits(SecretImage &secret_image, int message_length)
{
    std::vector<int> LSB_array;
    // TODO: Your code goes here.
    // 1. Reconstruct the SecretImage to a GrayscaleImage.
    // 2. Calculate the image dimensions.
    // 3. Determine the total bits required based on message length.
    // 4. Ensure the image has enough pixels; if not, throw an error.
    // 5. Calculate the starting pixel from the message_length knowing that
    //    the last LSB to extract is in the last pixel of the image.
    // 6. Extract LSBs from the image pixels and return the result.

    GrayscaleImage reconstructes_img = secret_image.reconstruct();

    int width = reconstructes_img.get_width();
    int height = reconstructes_img.get_height();
    int total_pix = width * height;
    int total_bits = message_length * 7;

    if (total_bits > total_pix)
    {
        throw std::runtime_error("Not enough pixels to extract the secret message");
    }

    int start_pixel = total_pix - total_bits;

    int bit_count = 0;

    for (int i = 0; i < height; ++i)
    {
        for (int j = 0; j < width; ++j)
        {
            if (bit_count >= start_pixel)
            {
                int pixel_val = reconstructes_img.get_pixel(i, j);
                int lsb = pixel_val & 1;

                LSB_array.push_back(lsb);
                 if (LSB_array.size() >= total_bits)
                {
                    return LSB_array;
                }
            }
            bit_count++;
        }
    }
    return LSB_array;
}

// Decrypt message by converting LSB array into ASCII characters
std::string Crypto::decrypt_message(const std::vector<int> &LSB_array)
{
    std::string message;
    // TODO: Your code goes here.

    // 1. Verify that the LSB array size is a multiple of 7, else throw an error.
    // 2. Convert each group of 7 bits into an ASCII character.
    // 3. Collect the characters to form the decrypted message.
    // 4. Return the resulting message.
    if (LSB_array.size() % 7 != 0)
    {
        throw std::invalid_argument("LSB array size is not a multiple of 7");
    }

    for (size_t i = 0; i < LSB_array.size(); i += 7)
    {
        int char_val = 0;
        for (int j = 0; j < 7; ++j)
        {
            char_val = (char_val << 1) | LSB_array[i + j];
        }
        message += static_cast<char>(char_val);
    }
    return message;
}

// Encrypt message by converting ASCII characters into LSBs
std::vector<int> Crypto::encrypt_message(const std::string &message)
{
    std::vector<int> LSB_array;
    // TODO: Your code goes here.

    // 1. Convert each character of the message into a 7-bit binary representation.
    //    You can use std::bitset.
    // 2. Collect the bits into the LSB array.
    // 3. Return the array of bits.
    for (char c : message)
    {
        std::bitset<7> bits(c);
        for (int i = 6; i >= 0; --i)
        {
            LSB_array.push_back(bits[i]);
        }
    }

    return LSB_array;
}

// Embed LSB array into GrayscaleImage starting from the last bit of the image
SecretImage Crypto::embed_LSBits(GrayscaleImage &image, const std::vector<int> &LSB_array)
{
    // TODO: Your code goes here.

    // 1. Ensure the image has enough pixels to store the LSB array, else throw an error.
    // 2. Find the starting pixel based on the message length knowing that
    //    the last LSB to embed should end up in the last pixel of the image.
    // 3. Iterate over the image pixels, embedding LSBs from the array.
    // 4. Return a SecretImage object constructed from the given GrayscaleImage
    //    with the embedded message.

    int total_pixels = image.get_width() * image.get_height();
    if (LSB_array.size() > total_pixels)
    {
        throw std::runtime_error("Not enough pixels to image to store the Lsb array");
    }

    int start_pixel_idx = total_pixels - LSB_array.size();

    int curr_pix = 0;
    for (int i = start_pixel_idx; i < total_pixels; ++i)
    {
        int row = i / image.get_width();
        int col = i % image.get_width();

        int original_pix = image.get_pixel(row, col);
        int bit_to_embed = LSB_array[curr_pix];
        if (bit_to_embed == 1)
        {
            original_pix |= 1;
        }
        else
        {
            original_pix &= ~1;
        }
        image.set_pixel(row, col, original_pix);
        curr_pix++;
    }
    SecretImage secret_image(image);
    return secret_image;
}
