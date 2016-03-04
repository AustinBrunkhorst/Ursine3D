//input texture we want to read from
Texture2D inputTexture : register(t0);
Texture2D depthTexture : register(t1);

//input of what position we want to get
cbuffer MousePosition : register(b0)
{
    int4 mousePos;
};

//defining the output to CPU
struct CS_OUTPUT
{
    uint id;
    uint x;
    uint y;
    float depth;
};

//specify the output to the CPU as a read-write buffer
RWStructuredBuffer<CS_OUTPUT> gOutput : register(u0);


//entry point, we only need 1 point
[numthreads(1, 1, 1)] 
void main()
{

    //mouse pos
    int mouseX = mousePos.x;
    int mouseY = mousePos.y;

    /////////////////////////////////////////////////////////////////
    // SEARCHING NEARBY TILES
    int searchRadius = 20;

    int xMin = mousePos.x - searchRadius;
    int xMax = mousePos.x + searchRadius;
    int yMin = mousePos.y - searchRadius;
    int yMax = mousePos.y + searchRadius;

    //best distance
    float bestDistance = 10000;

    //current ID to return
    uint bestAnswer = -1;
    uint num1 = -1;
    uint num2 = 0;

    uint xPos = 0, yPos = 0;

    bool currentStatus = false;

    //should we take overdraw into effect?
    bool usingOverdraw = false;

    //check surrounding area for overrides
    for (int y = yMin; y <= yMax; ++y)
        for (int x = xMin; x <= xMax; ++x)
        { 
            //read value from texture
            float4 currVal = inputTexture.Load(int3(x, y, 0));// [mousePos.xy];
            
            //convert to ID
            uint w1 = round(currVal.y * 255.f);
            uint w2 = round(currVal.z * 255.f);

            //construct final ID
            uint currID = w1 + (w2 << 8);

            //make sure ID is valid
            if (currID > 73727 || currID == 0)
            {
                continue;
            } 

            //calculate distance
            float distanceSqr = (x - mouseX) * (x - mouseX) + (y - mouseY) * (y - mouseY);

            //is the current ID an overdraw ID?
            int currIsOverdraw = ((currID >> 15) & 0x1);

            //if we want overdraw, but this object isn't, skip
            if (usingOverdraw && !currIsOverdraw)
                continue;

            //if current is overdraw but saved is not, automatically write
            if (!usingOverdraw && currIsOverdraw)
            {
                bestAnswer = currID;
                bestDistance = distanceSqr;
                usingOverdraw = true;

                num1 = w1;
                num2 = w2;

                xPos = x;
                yPos = y;
                continue;
            }

            //do we beat distance?
            if (distanceSqr < bestDistance)
            {
                bestAnswer = currID;
                bestDistance = distanceSqr;

                xPos = x;
                yPos = y;

                num1 = w1;
                num2 = w2;
                continue;
            }
        }

    // we now need to calculate other stuff, like the world position

    gOutput[ 0 ].id = bestAnswer; 
    gOutput[ 0 ].x = xPos;
    gOutput[ 0 ].y = yPos;
    gOutput[ 0 ].depth = depthTexture.Load( int3(xPos, yPos, 0) ).x;
    return;
}
