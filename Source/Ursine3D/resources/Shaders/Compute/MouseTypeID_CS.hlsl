//input texture we want to read from
Texture2D inputTexture : register(t0);

//input of what position we want to get
cbuffer MousePosition : register(b0)
{
    int4 mousePos;
};

//defining the output to CPU
struct CS_OUTPUT
{
    uint id;
};

//specify the output to the CPU as a read-write buffer
RWStructuredBuffer<CS_OUTPUT> gOutput : register(u0);


//entry point, we only need 1 point
[numthreads(1, 1, 1)] 
void main()
{
    //read value from texture
    float4 value = inputTexture.Load(int3(mousePos.x, mousePos.y, 0));// [mousePos.xy];

    float final = value.x + value.y + value.z + value.w;

    //grab the raw ID values
    uint finalID = value.y * 255.f;
    int size8_1 = value.z * 255.f;
    int size8_2 = value.w * 255.f;

    //shift values into place, combine to get the final ID
    gOutput[ 0 ].id = finalID;// +(size8_1 << 8) + (size8_2 << 16);
}
