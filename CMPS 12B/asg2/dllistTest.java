// Isaiah Solomon
// icsolomo
// 1344680
// CMPS 12B
// dllistTest.java
// Unit tests for dllist

import org.junit.*;
import static org.junit.Assert.assertEquals;

public class dllistTest {

	@Test
	public void startsEmptyTest() {
		dllist lst = new dllist();
    		assertEquals(true, lst.isEmpty());
	}
	
	@Test
	public void notEmptyTest() {
		dllist lst = new dllist();
		lst.insert("test", lst.first());
		assertEquals(false, lst.isEmpty());
	}

	@Test
	public void singleInsertLastTest() {
		dllist lst = new dllist();
		lst.insert("test", lst.last());
		assertEquals("test", lst.getItem());
	}
		
	@Test
	public void doubleInsertLastTest() {
		dllist lst = new dllist();
		lst.insert("test1", lst.last());
		lst.insert("test2", lst.last());
		assertEquals("test2", lst.getItem());
	}
		
	@Test
	public void singleInsertFirstTest() {
		dllist lst = new dllist();
		lst.insert("test", lst.first());
		assertEquals("test", lst.getItem());
	}
		
	@Test
	public void doubleInsertFirstTest() {
		dllist lst = new dllist();
		lst.insert("test1", lst.first());
		lst.insert("test2", lst.first());
		assertEquals("test2", lst.getItem());
	}
		
	@Test
	public void setPositionFirstTest() {
		dllist lst = new dllist();
		lst.insert("test1", lst.first());
		lst.insert("test2", lst.last());
		lst.setPosition(lst.first());
		assertEquals("test1", lst.getItem());
	}
		
	@Test
	public void setPositionLastTest() {
		dllist lst = new dllist();
		lst.insert("test1", lst.last());
		lst.insert("test2", lst.first());
		lst.setPosition(lst.last());
		assertEquals("test1", lst.getItem());
	}
		
	@Test
	public void setPositionPreviousTest() {
		dllist lst = new dllist();
		lst.insert("A", lst.last());
		lst.insert("B", lst.last());
		lst.insert("C", lst.last());
		lst.insert("D", lst.previous());
		lst.setPosition(lst.last());
		assertEquals("C", lst.getItem());
	}
		
	@Test
	public void setPositionFollowingTest() {
		dllist lst = new dllist();
		lst.insert("A", lst.first());
		lst.insert("B", lst.first());
		lst.insert("C", lst.first());
		lst.insert("D", lst.following());
		lst.setPosition(lst.first());
		assertEquals("C", lst.getItem());
	}
	
	@Test
	public void navigateItemTest() {
		dllist lst = new dllist();
		lst.insert("Hi", lst.last());
		lst.insert("Hiiii", lst.last());
		lst.insert("Hii", lst.last());
		lst.insert("Hiii", lst.last());
		lst.insert("hello", lst.last());
		lst.setPosition(lst.first());
		assertEquals("Hi", lst.getItem());
		lst.setPosition(lst.following());
		assertEquals("Hiiii", lst.getItem());
		lst.setPosition(lst.following());
		assertEquals("Hii", lst.getItem());
		lst.setPosition(lst.following());
		assertEquals("Hiii", lst.getItem());
		lst.setPosition(lst.following());
		assertEquals("hello", lst.getItem());
		lst.setPosition(lst.previous());
		assertEquals("Hiii", lst.getItem());
		lst.setPosition(lst.previous());
		assertEquals("Hii", lst.getItem());
		lst.setPosition(lst.previous());
		assertEquals("Hiiii", lst.getItem());
		lst.setPosition(lst.previous());
		assertEquals("Hi", lst.getItem());
	}

	@Test
	public void navigatePositionTest() {
		dllist lst = new dllist();
		lst.insert("Hi", lst.last());
		lst.insert("Hiiii", lst.last());
		lst.insert("Hii", lst.last());
		lst.insert("Hiii", lst.last());
		lst.insert("hello", lst.last());
		lst.setPosition(lst.first());
		assertEquals(0, lst.getPosition());
		lst.setPosition(lst.following());
		assertEquals(1, lst.getPosition());
		lst.setPosition(lst.following());
		assertEquals(2, lst.getPosition());
		lst.setPosition(lst.following());
		assertEquals(3, lst.getPosition());
		lst.setPosition(lst.following());
		assertEquals(4, lst.getPosition());
		lst.setPosition(lst.previous());
		assertEquals(3, lst.getPosition());
		lst.setPosition(lst.previous());
		assertEquals(2, lst.getPosition());
		lst.setPosition(lst.previous());
		assertEquals(1, lst.getPosition());
		lst.setPosition(lst.previous());
		assertEquals(0, lst.getPosition());
	}
	
	@Test
	public void singleItemDeleteTest() {
		dllist lst = new dllist();
		lst.insert("test", lst.first());
		lst.delete();
		assertEquals(true, lst.isEmpty());
	}
	
	@Test
	public void correctPositionDeleteTest() {
		dllist lst = new dllist();
		lst.insert("test1", lst.last());
		lst.insert("test2", lst.last());
		lst.insert("test3", lst.last());
		lst.setPosition(lst.previous());
		lst.delete();
		assertEquals("test3", lst.getItem());
	}
	
	@Test
	public void deleteLastItemTest() {
		dllist lst = new dllist();
		lst.insert("test1", lst.last());
		lst.insert("test2", lst.last());
		lst.insert("test3", lst.last());
		lst.delete();
		assertEquals("test2", lst.getItem());
	}
	
	@Test
	public void deleteFirstItemTest() {
		dllist lst = new dllist();
		lst.insert("test1", lst.last());
		lst.insert("test2", lst.last());
		lst.insert("test3", lst.last());
		lst.setPosition(lst.first());
		lst.delete();
		assertEquals("test2", lst.getItem());
	}
}
