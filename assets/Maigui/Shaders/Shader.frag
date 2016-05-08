#version 100
precision highp float;
varying vec2 TexCoord;
varying vec2 TexCoord2;

vec4 color;
uniform vec4 Color;

// Texture samplers
uniform sampler2D Texture1;

// original dimensions of the texture passed expressed in [top,bottom,]
uniform vec4 Range;
uniform vec2 Extension;
uniform vec2 Aspect;
// Texture extension based on aspect.
float Width = (Range[0] + Range[2])/2.0f;
float Height = (Range[1] + Range[3])/2.0f;

vec2 Top = vec2(Range[0],Range[1]);
vec2 Bottom = vec2(Range[2],Range[3]);


vec4 drawCorner(int corner) // corner in between 1 to 4
{
	vec2 offset = vec2(0.0f,0.0f);
	switch (corner)
	{
		// top left
		case 1:
		{
			offset.y = -((1.0f*Aspect.y) + (Top.y - Bottom.y));
			break;
		}
		// top right
		case 2:
		{
			offset.x = -((1.0f*Aspect.x) + (Top.x - Bottom.x));
			offset.y = -((1.0f*Aspect.y) + (Top.y - Bottom.y));
			break;
		}
		// bottom left
		case 3:
		{
			break;
		}
		// bottom right 
		case 4:
		{
			offset.x = -((1.0f*Aspect.x) + (Top.x - Bottom.x));
			break;
		}
	}
	vec2 pos = (Top + ((TexCoord2)*Aspect)) + offset;
	return texture2D(Texture1,pos);
}
vec4 drawSide(int side)
{
	vec2 pos;
	switch(side)
	{
		// left 
		case 1:
		{
			pos = (Top + ((TexCoord2)*Aspect));
			pos.y = (Top.y + (Bottom.y - Top.y)/2.0f);
			break;
		}
		// right
		case 2:
		{
			vec2 offset = vec2(0.0f,0.0f);
			offset.x = -((1.0f*Aspect.x) + (Top.x - Bottom.x));
			pos = (Top + ((TexCoord2)*Aspect)) + offset;
			pos.y = (Top.y + (Bottom.y - Top.y)/2.0f);
			
			break;
		}
		// top
		case 3:
		{
			vec2 offset = vec2(0.0f,0.0f);
			offset.y = -((1.0f*Aspect.y) + (Top.y - Bottom.y));
			pos = (Top + ((TexCoord2)*Aspect)) + offset;
			pos.x = (Top.x + (Bottom.x - Top.x)/2.0f);
			
			break;
		}
		// bottom 
		case 4:
		{
			vec2 offset = vec2(0.0f,0.0f);
			pos = (Top + ((TexCoord2)*Aspect)) + offset;
			pos.x = (Top.x + (Bottom.x - Top.x)/2.0f);
			
			break;
		}
	}
	return texture2D(Texture1,pos);
}
vec4 drawMiddle()
{
	vec2 pos = vec2(0.0f,0.0f);
	pos.y = (Top.y + (Bottom.y - Top.y)/2.0f);
	pos.x = (Top.x + (Bottom.x - Top.x)/2.0f);
	return texture2D(Texture1,pos);
}
void main()
{
	
	// it needs to look like this
	//				  
	//	+---------------------------+
	//	|c1| 		 s3          |c2|
	//	|--+---------------------+--|
	//	|  |		    		 |	|
	//	|  |					 |  |
	//  |  |	         		 |  |
	//  |s1|		  m			 |s2|
	//  |  |		    	 	 |  |
	//	|  |		     		 |  |
	//	|--+---------------------+--|
	//	|c3|		 s4          |c4|
	//  +---------------------------+
	// lets assume that the w and h of the image used is 100 and 200.
	// and the size of the rectangle is 200,100
	// we now need to figure out the x and y cordinate extension
	// in this case x would be: 200/2 = 100 [middle]
	// with extension of 50 on each side to make up 200
	// so we can if this:
	//	vec2 extension = [squareWidth - imageWidth, squareHeight - imageHeight]
	// if (extension.x > 0)
	//{
	//	if (position.x > size.x/2  AND  position.x < size.x/2 + extension.x)
	//	{
	//		float diff = position.x - size.x/2; the middle cordinate 
	//		here we need to extend the last pixel a few more times.
	//		color = texture(ourTexture,TexCoord/2 - diff);
	//	};
	// we do the same for the y axis.
	//}
	// and y would be: 0
	
	if (Extension.x > 0.0f)
	{
		bool changed = false;
		if (TexCoord.x > (Width - Extension.x/2.0f) && TexCoord.x < (Width + Extension.x/2.0f))
		{
	
		}
		else if (TexCoord.x < (Width + Extension.x/2.0f) && TexCoord.x < (Width + Extension.x/2.0f))
		{
			// left border
			if (TexCoord.y > (Height - Extension.y/2.0f) && TexCoord.y < (Height + Extension.y/2.0f))
			{
				color = drawSide(1);
				
			}else if (TexCoord.y < (Height + Extension.y/2.0f) && TexCoord.y < (Height + Extension.y/2.0f))
			{
				color = drawCorner(3);
			}
			else
			{
				// bottom left 
				color = drawCorner(1);
			}
			
	
			changed = true;
		}
		else 
		{	
			if (TexCoord.y > (Height - Extension.y/2.0f) && TexCoord.y < (Height + Extension.y/2.0f))
			{
				color = drawSide(2);
			}else if (TexCoord.y < (Height + Extension.y/2.0f) && TexCoord.y < (Height + Extension.y/2.0f))
			{
				color = drawCorner(4);
			}else 
			{
				color = drawCorner(2);
			}
			changed = true;
			

		}
		

		if (TexCoord.y > (Height - Extension.y/2.0f) && TexCoord.y < (Height + Extension.y/2.0f))
		{
			
		}
		else if (TexCoord.y < (Height + Extension.y/2.0f) && TexCoord.y < (Height + Extension.y/2.0f))
		{
			if (TexCoord.x > (Width - Extension.x/2.0f) && TexCoord.x < (Width + Extension.x/2.0f))
			{
				color = drawSide(4);
			}
			changed = true;
		}
		else
		{
			if (TexCoord.x > (Width - Extension.x/2.0f) && TexCoord.x < (Width + Extension.x/2.0f))
			{
				color = drawSide(3);
			}
			changed = true;
		}
		
		if (!changed)
		{
			color = drawMiddle();
		}

	}else
	{
		color = texture(Texture1, TexCoord);
	}
	color = Color*color;
	gl_FragColor = color;
}