
#ifdef _MSC_VER
#define _CRT_SECURE_NO_WARNINGS
#endif

#include<iostream>
#include <stdlib.h>
#include <math.h>
#include <sstream> 
#include<string>
#include<GL/glut.h> 
#define PI 3.14

using namespace std;

typedef struct treenode {
	int data;
	int id;
	float x, y, r;
	struct treenode* left;
	struct treenode* right;
	struct treenode* parent;
}TREENODE;

TREENODE* root = NULL;
TREENODE* delnode = NULL;
int w, sub1, n, w2, sub2, sub3, sub4, inst;
int id = 0, se, item, mode = 0, i = 0, key, insele, delele; 

void init() {
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(0, 500, 0, 500);
	glMatrixMode(GL_MODELVIEW);
}

void add_node(TREENODE** t_root, TREENODE** current) {

	if (*t_root == NULL) {
		*t_root = *current;
		(*t_root)->parent = NULL;
		return;
	}
	if ((*current)->data < (*t_root)->data) {
		if ((*t_root)->left == NULL) {
			(*current)->parent = (*t_root);
			(*t_root)->left = (*current);
		}
		else {
			add_node(&((*t_root)->left), current);
		}
	}
	else if ((*current)->data > (*t_root)->data) {
		if ((*t_root)->right == NULL) {
			(*current)->parent = (*t_root);
			(*t_root)->right = *current;
		}
		else {
			add_node(&((*t_root)->right), current);
		}
	}
}
void accept_tree() {
	int ele;
	cout << "\nCREATING BST: For initial tree, enter the input nodes (enter 0 to start)\n";
	while (1) {
		fflush(stdin);
		cin >> ele;
		if (ele == 0) {
			break;
		}
		TREENODE* current = new TREENODE;
		current->data = ele;
		current->right = NULL;
		current->left = NULL;
		current->parent = NULL;
		current->id = id;
		id += 1;
		n += 1;
		add_node(&root, &current);
	}
}

void draw_text(string temp, float x, float y) {
	glColor3f(0, 0, 0);
	char *text = new char[temp.length() + 1];
	strcpy(text, temp.c_str());
	GLvoid *font_style = GLUT_BITMAP_HELVETICA_18;
	glRasterPos3f(x, y, 0);
	for (int i = 0; text[i] != '\0'; i++) {
		glutBitmapCharacter(font_style, text[i]);
	}
}

void drawCircle(float segments, float radius, float sx, float sy)
{
	float theta = 2 * 3.1415926 / segments;
	float tan_factor = tanf(theta);
	float radial_factor = cosf(theta);
	float x = radius;
	float y = 0;
	int cache_pt = 0;
	double cache_x;
	double cache_y;
	glBegin(GL_LINES);
	for (int ii = 0; ii < segments; ii++) {
		if (!cache_pt) {
			cache_x = x + sx;
			cache_y = y + sy;
			cache_pt = 1;
		}
		else {
			glVertex2f(x + sx, y + sy);
			cache_x = x + sx;
			cache_y = y + sy;
		}
		float tx = -y;
		float ty = x;
		x += tx * tan_factor;
		y += ty * tan_factor;
		x *= radial_factor;
		y *= radial_factor;
	}
	glEnd();
	glFlush();
}
void draw_line(float x1, float y1, float x2, float y2) {
	glBegin(GL_LINES);
	glVertex2f(x1, y1);
	glVertex2f(x2, y2);
	glEnd();
}
void drawNode(TREENODE* t_root, float x1, float y1, int level) {
	if (t_root == NULL) {
		return;
	}
	float segments = 40;
	float radius = 1.0;
	float left_angle = 245;
	float right_angle = 115;
	float branch_length = 12 - level * 2.5;
	float angle_change = 20;
	root->x = x1;
	root->y = y1;
	root->r = radius;
	drawCircle(segments, radius, x1, y1);
	char buff[5];
	_itoa_s(t_root->data, buff, 10);
	draw_text(buff, x1, y1);
	if (t_root->left) {
		float angle = left_angle - level * angle_change;
		double radian = angle * 3.14 / 180;
		float m = (double)tan((double)radian);
		float x2 = x1 + branch_length * sin((double)radian);
		float y2 = y1 + branch_length * cos((double)radian);
		drawNode(t_root->left, x2, y2, level + 1);
		draw_line(x1, y1, x2, y2);
	}
	if (t_root->right) {
		float angle = right_angle + level * angle_change;
		float radian = angle * 3.14 / 180;
		float m = (double)tan((double)radian);
		float x2 = x1 + branch_length * sin((double)radian);
		float y2 = y1 + branch_length * cos((double)radian);
		drawNode(t_root->right, x2, y2, level + 1);
		draw_line(x1, y1, x2, y2);
	}

}

int search(TREENODE* tr) {
	if (tr != NULL) {
		if (tr->data == key) {
			glutSetWindow(sub1);
			glBegin(GL_LINE_LOOP);
			glVertex2f(1, 0);
			glVertex2f(499, 0);
			glVertex2f(499, 499);
			glVertex2f(1, 499);
			glEnd();
			draw_text("OUTPUT", 5, 350);
			draw_text("element found", 30, 100);
			draw_text(to_string(tr->data), 10, 100);
			return 1;
		}
		else if (tr->data < key)
			search(tr->right);
		else search(tr->left);
	}
	else {
		glutSetWindow(sub1);
		glBegin(GL_LINE_LOOP);
		glVertex2f(1, 0);
		glVertex2f(499, 0);
		glVertex2f(499, 499);
		glVertex2f(1, 499);
		glEnd();
		draw_text("OUTPUT", 5, 350);
		draw_text(to_string(key), 10, 100);
		draw_text("element not found", 30, 100);
		return 0;
	}
}
void keyboard3(unsigned char k, int x, int y)
{
	fflush(stdin);
	key = k - 48;
	cout << key << " ";
	if (key == -35) {
		glutSetWindow(w);
		glutPostRedisplay();
		glutSetWindow(sub3);
		glutHideWindow();
		glutSetWindow(sub1);
		glutPostRedisplay();
	}
	glutSetWindow(sub3);
	glColor3f(1, 0, 0);

	search(root);

	glutSwapBuffers();
	glClear(GL_COLOR_BUFFER_BIT);
}
void searchmouse(int bt, int st, int x, int y) {
	if (bt == GLUT_LEFT_BUTTON && st == GLUT_DOWN) {
		glutSetWindow(sub3);
		glutKeyboardFunc(keyboard3);
	}
	if (bt == GLUT_RIGHT_BUTTON && st == GLUT_DOWN) {
		se = 0;
		glutHideWindow();
		glutSetWindow(sub1);
		glutPostRedisplay();
	}
}
void disp() {
	if (se == 5)
		glutHideWindow();
	glClearColor(0.87, 0.87, 0.87, 1);
	glClear(GL_COLOR_BUFFER_BIT);
	glColor3f(0, 0, 0);
	glBegin(GL_LINE_LOOP);
	glVertex2f(1, 0);
	glVertex2f(499, 0);
	glVertex2f(499, 499);
	glVertex2f(1, 499);
	glEnd();
	draw_text("click & hover here to enter search key", 5, 160);
	glFlush();
	glutSwapBuffers();
}

void inorder(TREENODE* root) {
	if (root != NULL) {
		inorder(root->left);
		cout << root->data << " id:" << root->id << " ";
		cout << root->x << " " << root->y << endl;
		int ele = root->data;
		ostringstream str1;
		str1 << ele;
		string st = to_string(ele);
		draw_text(st, 10 + i, 100);
		i += 15;
		inorder(root->right);

	}
}
void preorder(TREENODE* root) {
	if (root != NULL) {
		cout << root->data << " ";
		glutSetWindow(sub1);
		int ele = root->data;
		ostringstream str1;
		str1 << ele;
		string st = to_string(ele);
		draw_text(st, 10 + i, 100);
		i += 15;
		preorder(root->left);
		preorder(root->right);
	}
}
void postorder(TREENODE* root) {
	if (root != NULL) {
		postorder(root->left);
		postorder(root->right);
		cout << root->data << " ";
		int ele = root->data;
		ostringstream str1;
		str1 << ele;
		string st = to_string(ele);
		draw_text(st, 10 + i, 100);
		i += 15;
	}
}
void disp2() {
	glClearColor(1, 1, 1, 1);
	glClear(GL_COLOR_BUFFER_BIT);
	glColor3f(0, 0, 0);
	glBegin(GL_LINE_LOOP);
	glVertex2f(1, 0);
	glVertex2f(499, 0);
	glVertex2f(499, 499);
	glVertex2f(1, 499);
	glEnd();
	draw_text("OUTPUT", 5, 350);
	switch (se) {
	case 1: draw_text("INORDER", 5, 225);
		inorder(root);
		break;
	case 2: draw_text("PREORDER", 5, 225);
		preorder(root);
		break;
	case 3: draw_text("POSTORDER", 5, 225);
		postorder(root);
		break;
	}
	glFlush();
	glutSwapBuffers();
}

void keyboard(unsigned char key, int x, int y)
{
	item = key - 48;
	fflush(stdin);
	cout << item << " ";
	if (item == -35) {
		glutSetWindow(w);
		glutPostRedisplay();
	}
	else {
		insele = item;
		glutSetWindow(sub2);
		draw_text(to_string(item), 5, 160);
	}
	if (insele!=-35 && item==-35)
	{
		cout << "yes";
		glClear(GL_COLOR_BUFFER_BIT);
		TREENODE* current = new TREENODE;
		current->data = insele;
		current->right = NULL;
		current->left = NULL;
		current->parent = NULL;
		current->id = id++;
		add_node(&root, &current);
		glutSetWindow(sub2);
		draw_text(to_string(insele), 5, 160);
		glFlush();
	}
	glutSwapBuffers();
	glClear(GL_COLOR_BUFFER_BIT);
}
void addmouse(int bt, int st, int x, int y) {
	if (bt == GLUT_LEFT_BUTTON && st == GLUT_DOWN) {
		glutSetWindow(sub2);
		glutKeyboardFunc(keyboard);
	}
	if (bt == GLUT_RIGHT_BUTTON && st == GLUT_DOWN) {
		glutPostRedisplay();
	}
}
void disp3() {
	glClearColor(0.87, 0.87, 0.87, 1);
	glClear(GL_COLOR_BUFFER_BIT);
	glColor3f(0, 0, 0);
	glBegin(GL_LINE_LOOP);
	glVertex2f(1, 0);
	glVertex2f(499, 0);
	glVertex2f(499, 499);
	glVertex2f(1, 499);
	glEnd();
	draw_text("ADD NODE", 5, 160);
	glFlush();
	glutSwapBuffers();
}

TREENODE* treemin(TREENODE* tr) {
	while (tr->left != NULL)
		tr = tr->left;
	return tr;
}
void transplant(TREENODE* t, TREENODE* u, TREENODE* v) {
	if (u->parent == NULL)
		root = v;
	else if (u == u->parent->left)
		u->parent->left = v;
	else u->parent->right = v;
	if (v != NULL)
		v->parent = u->parent;
}
void nodedelete(TREENODE* t, TREENODE* z) {
	TREENODE* y;
	if (z->left == NULL)
		transplant(t, z, z->right);
	else if (z->right == NULL)
		transplant(t, z, z->left);
	else {
		y = treemin(z->right);
		if (y->parent != z) {
			transplant(t, y, y->right);
			y->right = z->right;
			y->right->parent = y;
		}
		transplant(t, z, y);
		y->left = z->left;
		y->left->parent = y;
	}
}
int searchnode(TREENODE* rt) {
	if (rt == NULL)
		return 0;
	else {
		if (rt->data > delele)
			searchnode(rt->left);
		else if (rt->data < delele)
			searchnode(rt->right);
		else {
			delnode = rt;
			cout << "delete node : " << delnode->data << " ";
			return 1;
		}
	}
}
void keyboard2(unsigned char k, int x, int y)
{
	item = k - 48;
	fflush(stdin);
	cout << item << " ";
	if (item == -35) {
		glutSetWindow(w);
		glutPostRedisplay();
	}
	else {
		delele = item;
		draw_text(to_string(item), 5, 160);
	}
	if (searchnode(root)) {
		if (item == -35) {
			nodedelete(root, delnode);
			glutSetWindow(sub4);
			glutPostRedisplay();
		}
		else
			draw_text("Found", 100, 160);
	}
	else
		draw_text("NOT Found", 100, 160);

	glutSwapBuffers();
	glClear(GL_COLOR_BUFFER_BIT);
}
void delmouse(int bt, int st, int x, int y) {
	if (bt == GLUT_LEFT_BUTTON && st == GLUT_DOWN) {
		glutSetWindow(sub4);
		glutKeyboardFunc(keyboard2);
	}
	if (bt == GLUT_RIGHT_BUTTON && st == GLUT_DOWN) {
		glutPostRedisplay();
	}
}
void disp4() {
	glClearColor(0.87, 0.87, 0.87, 1);
	glClear(GL_COLOR_BUFFER_BIT);
	glColor3f(0, 0, 0);
	glBegin(GL_LINES);
	glVertex2f(2, 0);
	glVertex2f(499, 0);
	glVertex2f(499, 499);
	glVertex2f(2, 499);
	glEnd();
	draw_text("DELETE NODE", 5, 160);
	glFlush();
	glutSwapBuffers();
}

void win2menu(int c) {
	switch (c) {
	case 1: glutSetWindow(sub1);
		glutDestroyWindow(sub1);
		i = 0;
		mode = 0;
		break;
	}
}
void choice(int ch) {
	if (mode == 1) {
		glutDestroyWindow(sub1);
		mode = 0;
		i = 0;
	}
	switch (ch) {
	case 1: cout << endl;
		se = 1;
		break;
	case  2: cout << endl;
		se = 2;
		break;
	case 3: cout << endl;
		se = 3;
		break;
	case 4:
		se = 4;
		glutSetWindow(sub3);
		glutShowWindow();
		break;
	case 5: glutHideWindow();
		glutSetWindow(inst);
		glutShowWindow();
		se = 5;
		break;
	case 6: exit(0);
	}
	if (ch != 5) {
		sub1 = glutCreateSubWindow(w, 0, 700, 1000, 100);
		init();
		glutCreateMenu(win2menu);
		if (se != 4)
			glutAddMenuEntry("close", 1);
		glutAttachMenu(GLUT_RIGHT_BUTTON);
		glutDisplayFunc(disp2);
		mode = 1;
	}
}
void display(void) {

	glClearColor(0.7, 0.95, 1, 0.8);
	glClear(GL_COLOR_BUFFER_BIT);
	glLoadIdentity();

	glTranslatef(0, 10, -30);

	glColor3f(0, 0, 0);
	char text[] = "Binary Search Tree Operations";
	GLvoid *font_style = GLUT_BITMAP_TIMES_ROMAN_24;
	glRasterPos3f(-5, 5, 0);
	for (int i = 0; text[i] != '\0'; i++) {
		glutBitmapCharacter(font_style, text[i]);
	}
	drawNode(root, 0, 3, 0);
	glutCreateMenu(choice);
	glutAddMenuEntry("Inorder", 1);
	glutAddMenuEntry("Preorder", 2);
	glutAddMenuEntry("Postorder", 3);
	glutAddMenuEntry("Search node", 4);
	glutAddMenuEntry("INSTRUCTIONS", 5);
	glutAddMenuEntry("Exit", 6);
	glutAttachMenu(GLUT_RIGHT_BUTTON);
	glutSwapBuffers();
}


void mouseInstr(int bt, int st, int x, int y) {
	if (bt == GLUT_RIGHT_BUTTON && st == GLUT_DOWN) {
		glutHideWindow();
		glutSetWindow(w);
		glutShowWindow();
		//glutDestroyWindow(inst);
	}
}
void dispInstr() {
	glClearColor(1, 1, 1, 1);
	glClear(GL_COLOR_BUFFER_BIT);
	//glutFullScreen();

	glColor3f(0, 0, 0);

	char text[] = "INSTRUCTIONS";

	GLvoid *font_style = GLUT_BITMAP_HELVETICA_18;
	glRasterPos3f(200, 480, 0);
	for (int i = 0; text[i] != '\0'; i++) {
		glutBitmapCharacter(font_style, text[i]);
	}
	
	draw_text("This project simulates the behaviour of a binary search tree. It allows the user to add, delete and search for a particular node.",10, 450);
	draw_text("It also allows the user to traverse in three different modes, namely, inorder traversal, preorder traversal and postorder traversal.", 10, 430);
	draw_text("=> RIGHT CLICK to go to the BINARY SEARCH TREE OPERATIONS window.", 10, 390);
	draw_text("=> Left click on a sub window to activate it.", 10, 360);
	draw_text("=> Right click on the sub window to refresh it.", 10, 330);
	draw_text("=> Right click on the main(BST) window provides you with a menu.", 10, 300);
	draw_text("=> HOVER on the particular sub window to enter the input, and press enter to perform the operation.", 10, 270);
	
	draw_text("NOTE : THE TREE TAKES ONLY SINGLE DIGIT NUMBERS, AND CAN GENERATE NODES UPTO LEVEL 4.", 10, 230);

	glFlush();
}
void reshape(int w, int h) {
	glViewport(0, 0, (GLsizei)w, (GLsizei)h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(60, (GLfloat)w / (GLfloat)h, 0.1, 100.0);
	glMatrixMode(GL_MODELVIEW);
}

int main(int argc, char **argv) {

	accept_tree();

	glutInit(&argc, argv);
	
	glutInitDisplayMode(GLUT_SINGLE);
	glutInitWindowSize(1920, 1080);
	glutInitWindowPosition(0, 0);
	inst = glutCreateWindow("Instructions");
	init();
	glutDisplayFunc(dispInstr);
	glutMouseFunc(mouseInstr);
	
	glutInitDisplayMode(GLUT_DOUBLE);

	glutInitWindowSize(1920, 1080);
	glutInitWindowPosition(0, 0);
	w = glutCreateWindow("A Binary search tree");
	glutDisplayFunc(display);
	glutReshapeFunc(reshape);
	glutHideWindow();

	sub2 = glutCreateSubWindow(w, 1350, 100, 130, 30);
	glutSetWindow(sub2);
	init();
	glutDisplayFunc(disp3);
	glutMouseFunc(addmouse);

	sub4 = glutCreateSubWindow(w, 1300, 150, 180, 30);
	glutSetWindow(sub4);
	init();
	glutDisplayFunc(disp4);
	glutMouseFunc(delmouse);

	sub3 = glutCreateSubWindow(w, 2, 660, 350, 40);
	init();
	glutDisplayFunc(disp);
	glutMouseFunc(searchmouse);
	glutHideWindow();

	glutMainLoop();

	return 0;
}