#pragma once

#include "EngineTypes.h"
#include "Engine.h"

class HexagonalBlock
{
	
public:
	std::array<Vertex, 38> vertices;
	std::array<uint32_t, 72> indices;
	glm::mat4 defaultMatrix = glm::mat4(1.f);
	glm::mat4 inverseMatrix = glm::mat4(1.f);

	HexagonalBlock()
	{
		//default rotations
		defaultMatrix = glm::translate(defaultMatrix, glm::vec3(0, 0.5, 0));
		defaultMatrix = glm::rotate(defaultMatrix, glm::radians(-90.f), glm::vec3(0, 1, 0));
		defaultMatrix = glm::rotate(defaultMatrix, glm::radians(-90.f), glm::vec3(1, 0, 0));

		//inverse to translate to world space
		inverseMatrix = glm::rotate(defaultMatrix, glm::radians(90.f), glm::vec3(1, 0, 0));
		inverseMatrix = glm::rotate(inverseMatrix, glm::radians(90.f), glm::vec3(0, 1, 0));
		inverseMatrix = glm::translate(inverseMatrix, glm::vec3(0, -0.5, 0));


		//TOP FACE
		vertices[0].position = { 0, 0, 0 }; //center
		vertices[1].position = { 0.5f, 0, 0 }; // right
		vertices[2].position = { 0.25f, -0.25f * glm::sqrt(3), 0 }; // top right
		vertices[3].position = { -0.25f, -0.25f * glm::sqrt(3), 0 }; // top left
		vertices[4].position = { -0.5f, 0, 0 }; // left
		vertices[5].position = { -0.25f, 0.25f * glm::sqrt(3), 0 }; // bottom left
		vertices[6].position = { 0.25f, 0.25f * glm::sqrt(3), 0 }; // bottom right

		for (int i = 0; i < 7; i++)
		{
			vertices[i].textureID = 1;
		}

		vertices[0].uv_x = 0.5f;
		vertices[0].uv_y = 0.5f;

		vertices[1].uv_x = 1.0f;
		vertices[1].uv_y = 0.5f;

		vertices[2].uv_x = 0.75f;
		vertices[2].uv_y = 0.5f + 0.25f * glm::sqrt(3);

		vertices[3].uv_x = 0.25f;
		vertices[3].uv_y = 0.5f + 0.25f * glm::sqrt(3);

		vertices[4].uv_x = 0.0f;
		vertices[4].uv_y = 0.5f;

		vertices[5].uv_x = 0.25f;
		vertices[5].uv_y = 0.5f - 0.25f * glm::sqrt(3);

		vertices[6].uv_x = 0.75f;
		vertices[6].uv_y = 0.5f - 0.25f * glm::sqrt(3);


		indices[0] = 0;
		indices[1] = 1;
		indices[2] = 2;

		indices[3] = 0;
		indices[4] = 2;
		indices[5] = 3;

		indices[6] = 0;
		indices[7] = 3;
		indices[8] = 4;

		indices[9] = 0;
		indices[10] = 4;
		indices[11] = 5;

		indices[12] = 0;
		indices[13] = 5;
		indices[14] = 6;

		indices[15] = 0;
		indices[16] = 6;
		indices[17] = 1;

		//BOTTOM FACE
		vertices[7].position = { 0, 0, -0.5}; //center
		vertices[8].position = { 0.5f, 0, -0.5 }; // right
		vertices[9].position = { 0.25f, -0.25f * glm::sqrt(3), -0.5 }; // top right
		vertices[10].position = { -0.25f, -0.25f * glm::sqrt(3), -0.5 }; // top left
		vertices[11].position = { -0.5f, 0, -0.5 }; // left
		vertices[12].position = { -0.25f, 0.25f * glm::sqrt(3), -0.5 }; // bottom left
		vertices[13].position = { 0.25f, 0.25f * glm::sqrt(3), -0.5 }; // bottom right

		for (int i = 7; i < 14; i++)
		{
			vertices[i].textureID = 0;
		}

		vertices[7].uv_x = 0.5f;
		vertices[7].uv_y = 0.5f;

		vertices[8].uv_x = 1.0f;
		vertices[8].uv_y = 0.5f;

		vertices[9].uv_x = 0.75f;
		vertices[9].uv_y = 0.5f + 0.25f * glm::sqrt(3);

		vertices[10].uv_x = 0.25f;
		vertices[10].uv_y = 0.5f + 0.25f * glm::sqrt(3);

		vertices[11].uv_x = 0.0f;
		vertices[11].uv_y = 0.5f;

		vertices[12].uv_x = 0.25f;
		vertices[12].uv_y = 0.5f - 0.25f * glm::sqrt(3);

		vertices[13].uv_x = 0.75f;
		vertices[13].uv_y = 0.5f - 0.25f * glm::sqrt(3);


		indices[18] = 7;
		indices[19] = 8;
		indices[20] = 9;

		indices[21] = 7;
		indices[22] = 9;
		indices[23] = 10;

		indices[24] = 7;
		indices[25] = 10;
		indices[26] = 11;

		indices[27] = 7;
		indices[28] = 11;
		indices[29] = 12;

		indices[30] = 7;
		indices[31] = 12;
		indices[32] = 13;

		indices[33] = 7;
		indices[34] = 13;
		indices[35] = 8;


		//SIDES
		
		//R TR
		vertices[14].position = { 0.5f, 0, 0 };
		vertices[15].position = { 0.25f, -0.25f * glm::sqrt(3), 0 };
		vertices[16].position = { 0.5f, 0, -0.5 };
		vertices[17].position = { 0.25f, -0.25f * glm::sqrt(3), -0.5 };

		indices[36] = 17;
		indices[37] = 15;
		indices[38] = 14;

		indices[39] = 17;
		indices[40] = 14;
		indices[41] = 16;

		vertices[14].uv_x = 0.0f;
		vertices[14].uv_y = 1.0f;

		vertices[15].uv_x = 1.0f;
		vertices[15].uv_y = 1.0f;

		vertices[16].uv_x = 0.0f;
		vertices[16].uv_y = 0.0f;

		vertices[17].uv_x = 1.0f;
		vertices[17].uv_y = 0.0f;

		for (int i = 14; i < 18; i++)
		{
			vertices[i].textureID = 0;
		}

		//TR TL
		vertices[18].position = { 0.25f, -0.25f * glm::sqrt(3), 0 };
		vertices[19].position = { -0.25f, -0.25f * glm::sqrt(3), 0 };
		vertices[20].position = { 0.25f, -0.25f * glm::sqrt(3), -0.5 };
		vertices[21].position = { -0.25f, -0.25f * glm::sqrt(3), -0.5 };

		indices[42] = 21;
		indices[43] = 19;
		indices[44] = 18;

		indices[45] = 21;
		indices[46] = 18;
		indices[47] = 20;

		vertices[18].uv_x = 0.0f;
		vertices[18].uv_y = 1.0f;

		vertices[19].uv_x = 1.0f;
		vertices[19].uv_y = 1.0f;

		vertices[20].uv_x = 0.0f;
		vertices[20].uv_y = 0.0f;

		vertices[21].uv_x = 1.0f;
		vertices[21].uv_y = 0.0f;

		for (int i = 18; i < 22; i++)
		{
			vertices[i].textureID = 0;
		}

		//TL L

		vertices[22].position = { -0.25f, -0.25f * glm::sqrt(3), 0 };
		vertices[23].position = { -0.5f, 0, 0 };
		vertices[24].position = { -0.25f, -0.25f * glm::sqrt(3), -0.5 };
		vertices[25].position = { -0.5f, 0, -0.5f };

		indices[48] = 25;
		indices[49] = 23;
		indices[50] = 22;

		indices[51] = 25;
		indices[52] = 22;
		indices[53] = 24;

		vertices[22].uv_x = 0.0f;
		vertices[22].uv_y = 1.0f;

		vertices[23].uv_x = 1.0f;
		vertices[23].uv_y = 1.0f;

		vertices[24].uv_x = 0.0f;
		vertices[24].uv_y = 0.0f;

		vertices[25].uv_x = 1.0f;
		vertices[25].uv_y = 0.0f;

		for (int i = 22; i < 26; i++)
		{
			vertices[i].textureID = 0;
		}

		//L BL
		vertices[26].position = { -0.5f, 0, 0 };
		vertices[27].position = { -0.25f, 0.25f * glm::sqrt(3), 0 };
		vertices[28].position = { -0.5f, 0, -0.5f };
		vertices[29].position = { -0.25f, 0.25f * glm::sqrt(3), -0.5f };

		indices[54] = 29;
		indices[55] = 27;
		indices[56] = 26;

		indices[57] = 29;
		indices[58] = 26;
		indices[59] = 28;

		vertices[26].uv_x = 0.0f;
		vertices[26].uv_y = 1.0f;

		vertices[27].uv_x = 1.0f;
		vertices[27].uv_y = 1.0f;

		vertices[28].uv_x = 0.0f;
		vertices[28].uv_y = 0.0f;

		vertices[29].uv_x = 1.0f;
		vertices[29].uv_y = 0.0f;

		for (int i = 26; i < 30; i++)
		{
			vertices[i].textureID = 0;
		}

		//BL - BR
		vertices[30].position = { -0.25f, 0.25f * glm::sqrt(3), 0 };
		vertices[31].position = { 0.25f, 0.25f * glm::sqrt(3), 0 };
		vertices[32].position = { -0.25f, 0.25f * glm::sqrt(3), -0.5f };
		vertices[33].position = { 0.25f, 0.25f * glm::sqrt(3), -0.5f };

		indices[60] = 33;
		indices[61] = 31;
		indices[62] = 30;

		indices[63] = 33;
		indices[64] = 30;
		indices[65] = 32;

		vertices[30].uv_x = 0.0f;
		vertices[30].uv_y = 1.0f;

		vertices[31].uv_x = 1.0f;
		vertices[31].uv_y = 1.0f;

		vertices[32].uv_x = 0.0f;
		vertices[32].uv_y = 0.0f;

		vertices[33].uv_x = 1.0f;
		vertices[33].uv_y = 0.0f;

		for (int i = 30; i < 34; i++)
		{
			vertices[i].textureID = 0;
		}

		//BR R
		vertices[34].position = { 0.25f, 0.25f * glm::sqrt(3), 0 };
		vertices[35].position = { 0.5f, 0, 0 };
		vertices[36].position = { 0.25f, 0.25f * glm::sqrt(3), -0.5f };
		vertices[37].position = { 0.5f, 0, -0.5 };

		indices[66] = 37;
		indices[67] = 35;
		indices[68] = 34;

		indices[69] = 37;
		indices[70] = 34;
		indices[71] = 36;

		vertices[34].uv_x = 0.0f;
		vertices[34].uv_y = 1.0f;

		vertices[35].uv_x = 1.0f;
		vertices[35].uv_y = 1.0f;

		vertices[36].uv_x = 0.0f;
		vertices[36].uv_y = 0.0f;

		vertices[37].uv_x = 1.0f;
		vertices[37].uv_y = 0.0f;

		for (int i = 34; i < 38; i++)
		{
			vertices[i].textureID = 0;
		}
		/*
		vertices[14].position = { 0.5f, 0, 0 }; // right
		vertices[15].position = { 0.25f, -0.25f * glm::sqrt(3), 0 }; // top right
		vertices[17].position = { -0.25f, -0.25f * glm::sqrt(3), 0 }; // top left
		vertices[18].position = { -0.5f, 0, 0 }; // left
		vertices[19].position = { -0.25f, 0.25f * glm::sqrt(3), 0 }; // bottom left
		vertices[20].position = { 0.25f, 0.25f * glm::sqrt(3), 0 }; // bottom right

		
		vertices[22].position = { 0.5f, 0, -0.5 }; // right
		vertices[23].position = { 0.25f, -0.25f * glm::sqrt(3), -0.5 }; // top right
		vertices[24].position = { -0.25f, -0.25f * glm::sqrt(3), -0.5 }; // top left
		vertices[25].position = { -0.5f, 0, -0.5 }; // left
		vertices[26].position = { -0.25f, 0.25f * glm::sqrt(3), -0.5 }; // bottom left
		vertices[27].position = { 0.25f, 0.25f * glm::sqrt(3), -0.5 }; // bottom right

		*/

	}
};






