#pragma once
#include "primitives.h"

struct BoundingBox : public Box {

	vector<BoundingBox*> childBox;
	vector<Primitive*> content;
	int unboundedObjects = 0;
	

	BoundingBox() {};
	BoundingBox(Vec3 b0, Vec3 b1) {

		bounds[0] = b0;
		bounds[1] = b1;
		center = (bounds[0] + bounds[1]) / 2;
		update();

	};

	void addChildBox(Vec3 b0, Vec3 b1) {

		//cout << "b0 : " << b0 << " //// b1 : " << b1 << endl;
	
		childBox.push_back(new BoundingBox(b0,b1));
	}

	void resizeToFitContentBounds() {


		if (content.size() == 0) { return; }

		double xmin= 999999, ymin= 999999, zmin = 999999;
		double xmax = -999999, ymax = -999999, zmax = -999999;

		for (int i = 0; i < content.size(); i++) {
		
			if (content[i]->bounds[0].x < xmin) { xmin = content[i]->bounds[0].x; }
			if (content[i]->bounds[0].y < ymin) { ymin = content[i]->bounds[0].y; }
			if (content[i]->bounds[0].z < zmin) { zmin = content[i]->bounds[0].z; }
			if (content[i]->bounds[1].x > xmax) { xmax = content[i]->bounds[1].x; }
			if (content[i]->bounds[1].y > ymax) { ymax = content[i]->bounds[1].y; }
			if (content[i]->bounds[1].z > zmax) { zmax = content[i]->bounds[1].z; }
		
		}

		bounds[0] = Vec3(xmin, ymin, zmin);
		bounds[1] = Vec3(xmax, ymax, zmax);
		//cout << bounds[0] << " " << bounds[1]<<endl;
		center = (bounds[0] + bounds[1]) / 2;
		update();
	
	}

	void removeEmptySubBoxes() {

		for (int i = 0; i < childBox.size(); i++) {

			if (childBox[i]->content.size() == 0) {

				//cout << "remove empty" << endl;

				childBox.erase(childBox.begin() + i);
				i--;

			}

		}
	
	}



	void putContentInSubBoxes() {

		//cout << "content before : " << b->content.size();

		for (int j = 0; j < childBox.size(); j++) {
			for (int i = 0; i < content.size(); i++) {

				if (content[i]->isInside(childBox[j]) && content[i]->getType() != "plane") {

					childBox[j]->content.push_back(content[i]);

					content.erase(content.begin() + i);
					i--;
				}
			}
		}
		unboundedObjects = content.size();

		//cout << " // content after : " << b->content.size()<<endl;

	}

};